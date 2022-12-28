#!/bin/sh
set -e

#echo "Configuring User=${USER} with GID=${GID} and UID=${UID}"

#addgroup -g ${GID} ${USER}
#adduser -u ${UID} -G ${USER} -h ${PWD} -D ${USER}

#chown -R ${USER}.${USER} /home/${USER}

echo Your container args are: "$@"
#su ${USER} -c "/bin/sh"
su ${USER} -c "/bin/sh $@"
