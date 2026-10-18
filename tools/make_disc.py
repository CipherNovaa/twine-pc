"""Write a mkpsxiso project that uses the extracted disc but swaps in our built executables."""
import argparse
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("out_xml")
    ap.add_argument("--replace", nargs="*", default=[], help="DISCNAME=path, e.g. LEVEL01.EXE=build/level01/level01.exe")
    args = ap.parse_args()

    extracted = ROOT / "workbench/extracted"
    xml = (extracted / "disc.xml").read_text()
    xml = xml.replace('source="."', f'source="{extracted}"', 1)
    xml = xml.replace('file="./license_data.dat"', f'file="{extracted}/license_data.dat"')
    for rep in args.replace:
        name, path = rep.split("=", 1)
        pattern = re.compile(rf'(<file name="{re.escape(name)}" type="data")')
        xml, n = pattern.subn(rf'\1 source="{(ROOT / path).resolve()}"', xml)
        if n != 1:
            raise SystemExit(f"{name}: expected exactly one entry in disc.xml, found {n}")
    Path(args.out_xml).write_text(xml)


if __name__ == "__main__":
    main()
