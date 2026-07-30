import { connectToDaemon, getDaemonHost } from "../../utils/client.js";
/** Schema for update command output */
export const updateSchema = {
    idField: "agentId",
    columns: [
        { header: "AGENT ID", field: "agentId" },
        { header: "NAME", field: "name" },
        { header: "LABELS", field: "labels" },
        { header: "THINKING", field: "thinkingOptionId" },
        { header: "NOTICE", field: "notice" },
    ],
};
export function toAgentUpdateResult(agent, appliedChanges) {
    return {
        agentId: agent.id,
        name: agent.title,
        labels: formatLabels(agent.labels),
        thinkingOptionId: agent.effectiveThinkingOptionId ?? null,
        noticeType: appliedChanges.notice?.type ?? null,
        notice: appliedChanges.notice?.message ?? null,
    };
}
export async function applyAgentChanges(client, agentId, changes) {
    if (changes.type === "thinking") {
        // COMPAT(agentThinkingUpdate): added in v0.2.4, remove gate after 2027-01-28.
        if (client.getLastServerInfoMessage()?.features?.agentThinkingUpdate !== true) {
            throw {
                code: "DAEMON_UPDATE_REQUIRED",
                message: "Update the host to use agent thinking updates.",
            };
        }
        const notice = await client.setAgentThinkingOption(agentId, changes.thinkingOptionId);
        return { notice };
    }
    await client.updateAgent(agentId, changes.updates);
    return { notice: null };
}
function parseLabelOptions(labels) {
    const parsed = {};
    if (!labels) {
        return parsed;
    }
    for (const rawLabel of labels) {
        for (const segment of rawLabel.split(",")) {
            const label = segment.trim();
            if (!label) {
                continue;
            }
            const eqIndex = label.indexOf("=");
            if (eqIndex === -1) {
                const error = {
                    code: "INVALID_LABEL",
                    message: `Invalid label format: ${label}`,
                    details: "Labels must be in key=value format",
                };
                throw error;
            }
            const key = label.slice(0, eqIndex).trim();
            const value = label.slice(eqIndex + 1);
            if (!key) {
                const error = {
                    code: "INVALID_LABEL",
                    message: `Invalid label format: ${label}`,
                    details: "Labels must include a non-empty key in key=value format",
                };
                throw error;
            }
            parsed[key] = value;
        }
    }
    return parsed;
}
function formatLabels(labels) {
    const entries = Object.entries(labels);
    if (entries.length === 0) {
        return "-";
    }
    return entries.map(([key, value]) => `${key}=${value}`).join(",");
}
function parseAgentChanges(options) {
    const name = options.name?.trim();
    if (options.name !== undefined && !name) {
        throw {
            code: "INVALID_NAME",
            message: "Name cannot be empty",
            details: "Use --name <name> with a non-empty value",
        };
    }
    const labels = parseLabelOptions(options.label);
    const thinkingOptionId = options.thinking?.trim();
    if (options.thinking !== undefined && !thinkingOptionId) {
        throw {
            code: "INVALID_THINKING_OPTION",
            message: "--thinking cannot be empty",
            details: 'Provide a thinking option ID. Use "paseo provider models <provider> --thinking" to list valid IDs.',
        };
    }
    const hasMetadataUpdates = Boolean(name) || Object.keys(labels).length > 0;
    if (hasMetadataUpdates && thinkingOptionId) {
        throw {
            code: "INVALID_OPTIONS",
            message: "--thinking cannot be combined with --name or --label",
            details: "Run separate agent update commands for runtime settings and metadata.",
        };
    }
    if (!hasMetadataUpdates && !thinkingOptionId) {
        throw {
            code: "NO_CHANGES_PROVIDED",
            message: "Nothing to update",
            details: "Provide at least one of: --name <name>, --label <key=value>, --thinking <id>",
        };
    }
    if (thinkingOptionId) {
        return { type: "thinking", thinkingOptionId };
    }
    return {
        type: "metadata",
        updates: {
            ...(name ? { name } : {}),
            ...(Object.keys(labels).length > 0 ? { labels } : {}),
        },
    };
}
export async function runUpdateCommand(agentIdArg, options, _command) {
    const host = getDaemonHost({ host: options.host });
    // Validate arguments
    if (!agentIdArg || agentIdArg.trim().length === 0) {
        const error = {
            code: "MISSING_AGENT_ID",
            message: "Agent ID is required",
            details: "Usage: paseo agent update <id> [--name <name>] [--label <key=value>]",
        };
        throw error;
    }
    const changes = parseAgentChanges(options);
    let client;
    try {
        client = await connectToDaemon({ host: options.host });
    }
    catch (err) {
        const message = err instanceof Error ? err.message : String(err);
        const error = {
            code: "DAEMON_NOT_RUNNING",
            message: `Cannot connect to daemon at ${host}: ${message}`,
            details: "Start the daemon with: paseo daemon start",
        };
        throw error;
    }
    try {
        const fetchResult = await client.fetchAgent({ agentId: agentIdArg });
        if (!fetchResult) {
            const error = {
                code: "AGENT_NOT_FOUND",
                message: `Agent not found: ${agentIdArg}`,
                details: 'Use "paseo ls" to list available agents',
            };
            throw error;
        }
        const agentId = fetchResult.agent.id;
        const appliedChanges = await applyAgentChanges(client, agentId, changes);
        const updatedResult = await client.fetchAgent({ agentId });
        if (!updatedResult) {
            throw new Error(`Agent not found after update: ${agentId}`);
        }
        await client.close();
        return {
            type: "single",
            data: toAgentUpdateResult(updatedResult.agent, appliedChanges),
            schema: updateSchema,
        };
    }
    catch (err) {
        await client.close().catch(() => { });
        // Re-throw CommandError as-is
        if (err && typeof err === "object" && "code" in err) {
            throw err;
        }
        const message = err instanceof Error ? err.message : String(err);
        const error = {
            code: "UPDATE_FAILED",
            message: `Failed to update agent: ${message}`,
        };
        throw error;
    }
}
//# sourceMappingURL=update.js.map