#!/bin/sh
# The Windows build's last step of a module's partial link (port/Makefile): all but its descriptor made local, as on
# Linux, except its absolute symbols (PS1 data addresses, syms.ld): the PE linker resolves a relocation against a
# local absolute symbol to 0, so they stay global, renamed <module>.<name> (each module's own).
#   win_localize.sh OBJCOPY --keep-global-symbol=port_module_NAME IN OUT
set -e
OBJCOPY=$1
KEEP=$2
IN=$3
OUT=$4
MODULE=${KEEP#--keep-global-symbol=port_module_}
LIST=$OUT.abs
nm "$IN" | awk -v m="$MODULE" '$2 == "A" && $3 !~ /^\.weak\./ {print $3, m "." $3}' > "$LIST"
if [ -s "$LIST" ]; then
    awk '{print $2}' "$LIST" > "$LIST.keep"
    "$OBJCOPY" --redefine-syms="$LIST" "$KEEP" --keep-global-symbols="$LIST.keep" "$IN" "$OUT"
else # (objcopy fails on an empty list)
    "$OBJCOPY" "$KEEP" "$IN" "$OUT"
fi
rm -f "$LIST" "$LIST.keep"
