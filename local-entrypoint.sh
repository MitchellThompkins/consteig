#!/bin/sh
set -e

echo "Configuring User=${USER} with MY_GID=${MY_GID} and MY_UID=${MY_UID}"

addgroup -o -g ${MY_GID} ${USER}
adduser -u ${MY_UID} -G ${USER} -h ${PWD} -D ${USER}

chown -R ${USER}.${USER} /home/${USER}

su ${USER} -c "$@"
