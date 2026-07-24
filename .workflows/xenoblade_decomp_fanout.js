export const meta = {
  name: "xenoblade_decomp_fanout",
  description:
    "Xenoblade Chronicles Wii decompilation: read targets.json, fan out PPC assembly functions to autonomous agents that claim, decompile, cycle, iterate, and release targets",
  phases: [
    { title: "Prepare batch" },
    { title: "Fan out decompilation" },
    { title: "Synthesize results" },
  ],
};

// ── Config ─────────────────────────────────────────────────────────────────
const cfgLimit = args && typeof args.limit === "number" ? args.limit : null;
const cfgStatuses = args && Array.isArray(args.statuses) ? args.statuses : ["DISCOVERY", "BACKLOG"];

// ── Phase 0: Agent reads targets.json and prepares the batch ───────────────
phase("Prepare batch");

const batchResult = await agent(
  "Read the Xenoblade decompilation target registry and return matching targets.\n"
    + "\n"
    + "Project root: /Users/ahmedelbannan/Ahmed/xenoblade\n"
    + "Registry: tools/coop/targets.json\n"
    + "\n"
    + "1. Read the registry file.\n"
    + "2. Filter targets where workflow_status is one of: " + JSON.stringify(cfgStatuses) + ".\n"
    + "3. Sort: BACKLOG first, then DISCOVERY.\n"
    + "4." + (cfgLimit ? " Take only the first " + cfgLimit + " targets." : " Include all matching targets.") + "\n"
    + "5. Return an object with a 'targets' array.\n"
    + "\n"
    + "For each target include ONLY: id, function, symbol, address, size, source, unit, workflow_status, status, called_functions.\n"
    + "Skip targets missing 'source' or 'symbol'. Do NOT read any other files.",
  {
    label: "prepare-batch",
    schema: {
      type: "object",
      properties: {
        targets: {
          type: "array",
          items: {
            type: "object",
            properties: {
              id: { type: "string" },
              function: { type: "string" },
              symbol: { type: "string" },
              address: { type: "string" },
              size: { type: "string" },
              source: { type: "string" },
              unit: { type: "string" },
              workflow_status: { type: "string" },
              status: { type: "string" },
              called_functions: { type: "array", items: { type: "string" } },
            },
            required: ["id", "function", "symbol", "source", "unit"],
          },
        },
      },
      required: ["targets"],
    },
  },
);

const work = batchResult && Array.isArray(batchResult.targets) ? batchResult.targets : [];
if (work.length === 0) {
  return { summary: "No matching targets found", count: 0 };
}

const unitCount = new Set(work.map((w) => w.unit)).size;
log("Prepared " + work.length + " targets across " + unitCount + " units");

// ── Phase 1: Fan out — each agent autonomously decompiles and iterates ─────
phase("Fan out decompilation");

function buildPrompt(target) {
  return "You are a PPC (PowerPC 32-bit big-endian, Broadway) to C/C++ decompiler for Xenoblade Chronicles Wii.\n"
    + "The target compiler is Metrowerks CodeWarrior (MWCC).\n"
    + "Project root: /Users/ahmedelbannan/Ahmed/xenoblade\n"
    + "\n"
    + "## Your task\n"
    + "Decompile the function below to high-level C or C++ by running the project's toolchain to verify. Do NOT just return code - iterate until the function matches.\n"
    + "\n"
    + "## Function metadata\n"
    + "- ID: " + target.id + "\n"
    + "- Name: " + target.function + "\n"
    + "- Symbol (mangled): " + target.symbol + "\n"
    + "- Address: " + target.address + "\n"
    + "- Size: " + target.size + "\n"
    + "- Source file: " + target.source + "\n"
    + "- Translation unit: " + target.unit + "\n"
    + "- Called function IDs: " + JSON.stringify(target.called_functions || []) + "\n"
    + "\n"
    + "## Step-by-step\n"
    + "\n"
    + "### Step 1: Read assembly\n"
    + 'read("build/us/asm/' + target.unit + '.s")\n'
    + "Find the .fn section for " + target.symbol + ".\n"
    + "\n"
    + "### Step 2: Read context\n"
    + 'read("' + target.source + '")\n'
    + "Read the source file and any headers.\n"
    + "\n"
    + "### Step 3: Claim the target\n"
    + 'bash: cd /Users/ahmedelbannan/Ahmed/xenoblade && python3 tools/coop/run.py targets claim ' + target.id + ' --owner "pi:decomp:' + target.id + '"\n'
    + "\n"
    + "### Step 4: Write C/C++ implementation\n"
    + "- Use struct members, parameters, return values, if/else, loops, helper calls\n"
    + "- Match the function signature from the mangled symbol name\n"
    + "- Express control flow naturally (if/else, switch, for/while) - not goto chains\n"
    + "- External globals (lbl_*, sda21 refs) may be extern C globals\n"
    + "- REPLACE any asm void placeholder body with real high-level C/C++\n"
    + "- ADD the function if no body exists yet\n"
    + "- If source already has a working C/C++ body (not asm void), KEEP it and edit for correctness\n"
    + "- NEVER write asm void, nofralloc, or asm() blocks - FORBIDDEN by project policy, will fail the build\n"
    + "\n"
    + "### Step 5: Run cycle\n"
    + 'bash: cd /Users/ahmedelbannan/Ahmed/xenoblade && python3 tools/coop/run.py cycle ' + target.id + ' --hypothesis "decompiled function" --next-change "initial decompilation"\n'
    + "\n"
    + "Read the cycle output. Cycle runs: ctx + build + diff.\n"
    + "- Status 0: function matches. Go to Step 7.\n"
    + "- Failure: read diff output, go to Step 6.\n"
    + "\n"
    + "### Step 6: Iterate (up to 3 total)\n"
    + "Based on cycle/diff output, revise code. Check: signed vs unsigned, struct layout, expression order.\n"
    + "After revising, run cycle again. Max 3 attempts total.\n"
    + "\n"
    + "### Step 7: Release and report\n"
    + 'bash: cd /Users/ahmedelbannan/Ahmed/xenoblade && python3 tools/coop/run.py targets release ' + target.id + "\n"
    + "\n"
    + "## Constraints\n"
    + "- Do NOT explore the DOL binary, verify addresses, or analyze the build system\n"
    + "- Do NOT modify files outside the target's source file\n"
    + "- Do NOT write asm() blocks, asm void, nofralloc, register variables, or stack tricks - FORBIDDEN\n"
    + "- If source already has a working high-level C/C++ body, do NOT replace it with asm\n"
    + "- Run cycle early and iterate. Max 3 attempts per function.\n"
    + "- Use /Users/ahmedelbannan/Ahmed/xenoblade as working directory for all bash commands\n"
    + "\n"
    + "## Output format\n"
    + "Return a JSON object with:\n"
    + "- source_code: the final C/C++ function implementation\n"
    + "- hypothesis: brief explanation of what the function does\n"
    + "- next_change: summary of changes on the final attempt\n"
    + '- confidence: "high", "medium", or "low"\n'
    + "- attempts: number of decompilation iterations performed\n"
    + "- cycle_passed: true if cycle reported success, false otherwise\n"
    + "- blocker: describe remaining issue if failed (null if passed)";
}

const fanOutResults = await parallel(
  work.map((target, index) => {
    var label = "decomp:" + index + ":" + target.id.replace(/[^a-zA-Z0-9_-]/g, "_");
    return () => agent(
        buildPrompt(target),
        {
          label: label,
          schema: {
            type: "object",
            properties: {
              source_code: { type: "string", description: "Final C/C++ function implementation" },
              hypothesis: { type: "string", description: "What the function does" },
              next_change: { type: "string", description: "Summary of changes on final attempt" },
              confidence: { type: "string", enum: ["high", "medium", "low"] },
              attempts: { type: "number", description: "Number of decompilation iterations" },
              cycle_passed: { type: "boolean", description: "Did cycle report success?" },
              blocker: { type: "string", description: "If failed, describe the blocker (null if passed)" },
            },
            required: ["source_code", "hypothesis", "next_change", "confidence", "attempts", "cycle_passed"],
          },
        },
      );
  }),
);

// ── Build coverage ledger ───────────────────────────────────────────────────
var ledger = work.map((target, index) => ({
    id: target.id,
    function: target.function,
    symbol: target.symbol,
    source: target.source,
    unit: target.unit,
    status: fanOutResults[index] === null ? "failed" : "complete",
    result: fanOutResults[index],
  }));

var completed = ledger.filter((l) => l.status === "complete");
var failed = ledger.filter((l) => l.status === "failed");

log("Decomp complete: " + completed.length + " ok, " + failed.length + " failed");

// ── Phase 2: Synthesize ─────────────────────────────────────────────────────
phase("Synthesize results");

var synthesisSummary = completed.map((l) => {
  var r = l.result || {};
  return {
    id: l.id,
    function: l.function,
    confidence: r.confidence,
    cycle_passed: r.cycle_passed,
    attempts: r.attempts,
    hypothesis: r.hypothesis,
    blocker: r.blocker,
  };
});

var synthesis = await agent(
  "Synthesize the decompilation results. Summarize overall outcome, "
    + "count completions vs failures, distinguish cycle_passed from not-passed, "
    + "note patterns in blockers and confidence levels.\n\n"
    + JSON.stringify(synthesisSummary, null, 2),
  {
    label: "synthesize-decomp-ledger",
    schema: {
      type: "object",
      properties: {
        summary: { type: "string" },
        completedIds: { type: "array", items: { type: "string" } },
        failedIds: { type: "array", items: { type: "string" } },
        totalComplete: { type: "number" },
        totalFailed: { type: "number" },
        passed: { type: "number", description: "Count where cycle_passed === true" },
        notPassed: { type: "number", description: "Count where cycle_passed === false" },
        patterns: { type: "string" },
      },
      required: ["summary", "completedIds", "failedIds", "totalComplete", "totalFailed", "passed", "notPassed"],
    },
  },
);

return { ledger: ledger, synthesis: synthesis };
