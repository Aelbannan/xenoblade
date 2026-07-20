"""Shared JSON Schema for decomp candidate responses across providers."""

from __future__ import annotations

from typing import Any, Dict

# OpenAI / LM Studio style json_schema wrapper (name + strict + schema).
# OpenCode message `format.schema` also accepts this object.
CANDIDATE_JSON_SCHEMA: Dict[str, Any] = {
    "name": "decomp_candidate",
    "strict": True,
    "schema": {
        "type": "object",
        "properties": {
            "source": {
                "type": "string",
                "description": "Complete replacement high-level C/C++ function definition only",
            },
            "hypothesis": {
                "type": "string",
                "maxLength": 160,
                "description": "One short hypothesis (<=160 chars)",
            },
            "notes": {
                "type": "array",
                "maxItems": 3,
                "items": {"type": "string", "maxLength": 120},
                "description": "Up to 3 short notes (<=120 chars each)",
            },
            "next_change": {
                "type": "string",
                "maxLength": 120,
                "description": "One short follow-up (<=120 chars); empty if none",
            },
            "change": {
                "type": "string",
                "maxLength": 120,
                "description": "One short change summary (<=120 chars); empty for first candidates",
            },
        },
        "required": ["source", "hypothesis", "notes", "next_change", "change"],
        "additionalProperties": False,
    },
}
