from __future__ import annotations

import hashlib
from dataclasses import dataclass
from enum import Enum


class RegionKind(str, Enum):
    RAM = "ram"
    ROM_IMAGE = "rom-image"
    MMIO = "mmio"
    UNMAPPED = "unmapped"


@dataclass(frozen=True, slots=True)
class Region:
    start: int
    end: int
    kind: RegionKind
    image_bytes: bytes | None = None
    image_sha256: str | None = None
    label: str | None = None
    device_id: str | None = None

    def __post_init__(self) -> None:
        if self.start < 0 or self.end < 0 or self.start > 0xFFFFFFFF or self.end > 0xFFFFFFFF:
            raise ValueError(
                f"region [{self.start:#x}, {self.end:#x}] out of 32-bit address space"
            )
        if self.start > self.end:
            raise ValueError(
                f"region start {self.start:#x} > end {self.end:#x}"
            )
        if self.kind is RegionKind.ROM_IMAGE:
            if self.image_bytes is None:
                raise ValueError("ROM_IMAGE region requires image_bytes")
            expected_len = self.end - self.start + 1
            if len(self.image_bytes) != expected_len:
                raise ValueError(
                    f"ROM_IMAGE length {len(self.image_bytes)} != span {expected_len}"
                )
            if self.image_sha256 is None:
                object.__setattr__(
                    self,
                    "image_sha256",
                    hashlib.sha256(self.image_bytes).hexdigest(),
                )
        if self.device_id is not None and self.kind is not RegionKind.MMIO:
            raise ValueError("device_id is only valid for MMIO regions")


@dataclass(frozen=True, slots=True)
class RangeClassification:
    """Result of classifying a byte range [start, start + width)."""

    region: Region | None = None
    spans_multiple_regions: bool = False
    regions: tuple[Region, ...] = ()


@dataclass(frozen=True, slots=True)
class AddressSpace:
    regions: tuple[Region, ...]

    def __post_init__(self) -> None:
        ordered = tuple(sorted(self.regions, key=lambda r: (r.start, r.end)))
        for left, right in zip(ordered, ordered[1:]):
            if right.start <= left.end:
                raise ValueError(
                    "overlapping regions: "
                    f"[{left.start:#x}, {left.end:#x}] and "
                    f"[{right.start:#x}, {right.end:#x}]"
                )
        object.__setattr__(self, "regions", ordered)

    def classify(self, addr: int) -> Region | None:
        if addr < 0 or addr > 0xFFFFFFFF:
            raise ValueError(f"address {addr:#x} out of 32-bit address space")
        for region in self.regions:
            if region.start <= addr <= region.end:
                return region
        return None

    def classify_range(self, start: int, width: int) -> RangeClassification:
        if width <= 0:
            raise ValueError("width must be positive")
        if start < 0 or start > 0xFFFFFFFF:
            raise ValueError(f"start address {start:#x} out of 32-bit address space")
        last = start + width - 1
        if last < start or last > 0xFFFFFFFF:
            raise ValueError(
                f"range [{start:#x}, {last:#x}] wraps or exceeds 32-bit address space"
            )

        touched: list[Region] = []
        seen: set[tuple[int, int, RegionKind]] = set()
        saw_unmapped = False
        for addr in range(start, last + 1):
            region = self.classify(addr)
            if region is None:
                saw_unmapped = True
                continue
            key = (region.start, region.end, region.kind)
            if key not in seen:
                seen.add(key)
                touched.append(region)

        if not touched:
            return RangeClassification()
        if saw_unmapped or len(touched) > 1:
            return RangeClassification(
                spans_multiple_regions=True,
                regions=tuple(touched),
            )
        return RangeClassification(region=touched[0])


def rom_image_region(
    base: int,
    image_bytes: bytes,
    *,
    label: str | None = None,
) -> Region:
    """Build an immutable ROM/table image region covering ``image_bytes`` at ``base``."""
    if not image_bytes:
        raise ValueError("image_bytes must be nonempty")
    end = base + len(image_bytes) - 1
    return Region(
        start=base,
        end=end,
        kind=RegionKind.ROM_IMAGE,
        image_bytes=image_bytes,
        image_sha256=hashlib.sha256(image_bytes).hexdigest(),
        label=label,
    )


def mmio_region(
    start: int,
    end: int,
    *,
    device_id: str | None = None,
    label: str | None = None,
) -> Region:
    """Build an MMIO region descriptor (device bus wiring is future work)."""
    return Region(
        start=start,
        end=end,
        kind=RegionKind.MMIO,
        label=label,
        device_id=device_id,
    )


def attach_mmio_region(
    space: AddressSpace,
    start: int,
    end: int,
    *,
    device_id: str | None = None,
    label: str | None = None,
) -> AddressSpace:
    """Return a new ``AddressSpace`` with an additional MMIO region appended."""
    return AddressSpace(space.regions + (mmio_region(start, end, device_id=device_id, label=label),))
