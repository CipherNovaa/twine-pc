#!/bin/bash
# Endless mode's regression suite (headless runs of build/port/twine, checked from their logs and screenshots).
# Usage: tools/endless_check.sh [-j JOBS] [-k CHECKS] [-r ROOMS] [-v]   (details: tools/endless_check.py's header)
#   e.g. tools/endless_check.sh -k ab -r 2:5,6:13    two rooms, guards fight and nobody unkillable
exec python3 "$(dirname "$0")/endless_check.py" "$@"
