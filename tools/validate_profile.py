import json
import sys
from pathlib import Path


def main() -> int:
    if len(sys.argv) != 2:
        print("usage: python tools/validate_profile.py <profile.json>")
        return 1

    profile_path = Path(sys.argv[1])
    if not profile_path.exists():
        print(f"profile not found: {profile_path}")
        return 1

    with profile_path.open("r", encoding="utf-8") as f:
        data = json.load(f)

    required_keys = ["profileName", "input", "camera", "targeting", "quickbars"]
    missing = [k for k in required_keys if k not in data]
    if missing:
        print("missing keys:", ", ".join(missing))
        return 2

    print("profile structure validated")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
