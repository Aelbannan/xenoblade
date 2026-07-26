"""Provider factory and re-exports.

Usage::

    from tools.llm_decomp.providers import get_provider
    provider = get_provider("openrouter")
"""
from __future__ import annotations

from .base import Provider, ProviderError, ProviderReply

__all__ = [
    "Provider",
    "ProviderError",
    "ProviderReply",
    "get_provider",
]


def get_provider(name: str) -> Provider:
    """Return a provider adapter instance by name.

    Args:
        name: Provider identifier, e.g. ``"openrouter"``.

    Returns:
        A provider instance implementing the ``Provider`` protocol.

    Raises:
        ValueError: If the name is not recognised.
    """
    if name == "openrouter":
        from .openrouter import OpenRouterProvider

        return OpenRouterProvider()
    raise ValueError(
        f"Unknown provider {name!r}. Supported: openrouter"
    )