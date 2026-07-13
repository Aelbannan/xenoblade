from __future__ import annotations

import socket
import time
from typing import Optional


def _gdb_checksum(payload: str) -> str:
    return f"{sum(payload.encode('ascii')) % 256:02x}"


def _gdb_packet(command: str) -> bytes:
    return b"+" + f"${command}#{_gdb_checksum(command)}".encode("ascii")


def _gdb_recv_packet(sock: socket.socket) -> str:
    data = b""
    while b"#" not in data:
        chunk = sock.recv(4096)
        if not chunk:
            break
        data += chunk

    text = data.decode("ascii", errors="replace")
    start = text.rfind("$")
    if start < 0:
        return text
    end = text.find("#", start + 1)
    if end < 0:
        return text
    return text[start + 1 : end]


def wait_for_port(host: str, port: int, *, timeout_s: float = 20.0) -> bool:
    deadline = time.time() + timeout_s
    while time.time() < deadline:
        try:
            with socket.create_connection((host, port), timeout=0.5):
                return True
        except OSError:
            time.sleep(0.25)
    return False


def gdb_read_memory_at_exit(
    host: str,
    port: int,
    *,
    exit_addr: int,
    reads: list[tuple[int, int]],
    timeout_s: float = 20.0,
) -> dict[int, bytes] | None:
    """Run the guest under Dolphin's GDB stub, break at exit, read MEM1 regions."""
    deadline = time.time() + timeout_s
    sock: socket.socket | None = None

    while sock is None and time.time() < deadline:
        try:
            sock = socket.create_connection((host, port), timeout=1.0)
        except OSError:
            time.sleep(0.25)

    if sock is None:
        return None

    try:
        sock.settimeout(5.0)
        for bp in (
            f"Z0,{exit_addr:x},0",
            f"Z0,{exit_addr:x},4",
            f"Z1,{exit_addr:x},4",
        ):
            sock.sendall(_gdb_packet(bp))
            if _gdb_recv_packet(sock) != "OK":
                return None

        sock.sendall(_gdb_packet("c"))
        time.sleep(3.0)
        sock.settimeout(0.5)
        try:
            _gdb_recv_packet(sock)
        except OSError:
            pass
        sock.settimeout(None)

        out: dict[int, bytes] = {}
        for address, size in reads:
            sock.sendall(b"+")
            sock.sendall(_gdb_packet(f"m{address:x},{size:x}"))
            hex_data = _gdb_recv_packet(sock)
            if not hex_data or hex_data.startswith("E"):
                return None
            if len(hex_data) != size * 2:
                return None
            out[address] = bytes.fromhex(hex_data)
        return out
    except (OSError, ValueError):
        return None
    finally:
        sock.close()
