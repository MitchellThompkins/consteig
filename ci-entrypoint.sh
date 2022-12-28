#!/bin/sh
set -e

chown -R ${USER}.${USER} /home/${USER}

su ${USER} -c "$@"
