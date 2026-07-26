"""Abstract provider interface for llm_decomp.

Every provider adapter implements ``Provider`` (a typing.Protocol).
Design reference: docs/llm_decomp_design.md §13.
"""
from __future__ import annotations

from dataclasses import dataclass, field
from typing import Protocol

from tools.llm_decomp.contracts import ToolCall
from tools.llm_decomp.config import ModelConfig


class ProviderError(RuntimeError):
    """Raised by provider implementations on unrecoverable API or config errors.

    Includes missing API keys, HTTP-level failures after retries, and
    malformed API responses that cannot be parsed into a ProviderReply.
    """


@dataclass
class ProviderReply:
    """Structured response from a provider invocation.

    Attributes:
        text: The assistant's text content (may be empty if only tool_calls).
        tool_calls: Parsed tool call objects from the response.
        usage: Best-effort token counts, e.g. ``{'input_tokens': ..., 'output_tokens': ...}``.
        finish_reason: The reason the generation stopped: ``'stop'``, ``'length'``,
            ``'tool_calls'``, or a provider-specific passthrough.
    """
    text: str
    tool_calls: list[ToolCall] = field(default_factory=list)
    usage: dict = field(default_factory=dict)
    finish_reason: str = "stop"


class Provider(Protocol):
    """Protocol for LLM provider backends.

    All providers accept the OpenAI chat-completion message format and return
    a structured ProviderReply. The harness owns the loop; providers never
    touch the filesystem.
    """

    def send(
        self,
        messages: list[dict],
        tools: list[dict],
        model: ModelConfig,
    ) -> ProviderReply:
        """Send a conversation turn and return the assistant's response.

        Args:
            messages: OpenAI-format message list (role/content, plus
                role='tool' messages with tool_call_id).
            tools: OpenAI-format tool/function schemas. Empty list means
                tool calling is not available for this turn.
            model: Resolved model configuration.

        Returns:
            A ProviderReply with the assistant's text, any tool calls,
            usage information, and finish reason.

        Raises:
            ProviderError: On configuration errors, authentication failures,
                or unrecoverable API errors after retries.
        """
        ...


def compute_cost(usage: dict) -> dict:
    """Estimate USD cost from a usage dict for DeepSeek V4 Flash pricing.

    Rates (per 1M tokens):
        Input:      $0.09
        Output:     $0.18
        Cached in:  $0.009 (~90% discount)

    Returns {"input_cost", "cached_input_cost", "output_cost", "total_cost"}
    with 6-decimal precision. Zero values if usage is empty/missing.
    """
    inp = usage.get("input_tokens", 0) or 0
    cached = usage.get("cache_input_tokens", 0) or 0
    out = usage.get("output_tokens", 0) or 0
    return {
        "input_cost": round(inp * 0.09 / 1_000_000, 6),
        "cached_input_cost": round(cached * 0.009 / 1_000_000, 6),
        "output_cost": round(out * 0.18 / 1_000_000, 6),
        "total_cost": round(
            inp * 0.09 / 1_000_000
            + cached * 0.009 / 1_000_000
            + out * 0.18 / 1_000_000, 6),
    }