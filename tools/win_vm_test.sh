#!/bin/bash
# The Windows build in the Windows 11 test VM (~/VMs/twine-win, its README.txt): the USB test kit (port/usbkit) made
# into build/win-vm/kit, the disc image linked in (never copied into a kit or dist folder that leaves the machine:
# the link is removed again afterwards, and the copy in the VM deleted at the end of the job), run there from a clean VM state without its play step
# (TEST.bat noplay, in the desktop session), the results copied back to build/win-vm/results, the VM shut down, and
# compared with the same runs on Linux (tools/usbkit_compare.py).
#   tools/win_vm_test.sh [VM_DIR]
set -e
ROOT=$(cd "$(dirname "$0")/.." && pwd)
VM=${1:-$HOME/VMs/twine-win}
OUT=$ROOT/build/win-vm
DISC=${TWINE_DISC:-$(ls "$ROOT"/workbench/disc/*.bin | head -1)}
rm -rf "$OUT" && mkdir -p "$OUT"
make -C "$ROOT/port" usbkit DEST="$OUT/kit"
ln "$DISC" "$OUT/kit/disc/" 2>/dev/null || cp "$DISC" "$OUT/kit/disc/"
CUE="${DISC%.bin}.cue"
[ -f "$CUE" ] && { ln "$CUE" "$OUT/kit/disc/" 2>/dev/null || cp "$CUE" "$OUT/kit/disc/"; }
set +e
"$VM/run.sh" -s -i -t 1500 -o "$OUT/results" "$OUT/kit" \
    'call TEST.bat noplay & xcopy /e /i /y /q results "%TWINE_RESULTS%\kit" >nul & del /q disc\*.bin disc\*.cue'
rc=$?
rm -f "$OUT"/kit/disc/*.bin "$OUT"/kit/disc/*.cue
echo "win_vm_test: run.sh exit $rc; results in $OUT/results"
R=$(ls -d "$OUT"/results/kit/*/ 2>/dev/null | tail -1)
[ -n "$R" ] && python3 "$ROOT/tools/usbkit_compare.py" "$R"
exit $rc
