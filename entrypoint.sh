#!/bin/sh
set -e

echo "Configuring User=${USER} with MY_GID=${MY_GID} and MY_UID=${MY_UID}"

# 1. Determine the Group Name
# Check if the GID exists; if so, get its name. If not, create it.
IF_GROUP_EXISTS=$(getent group "${MY_GID}" | cut -d: -f1)

if [ -n "$IF_GROUP_EXISTS" ]; then
    GROUP_NAME=$IF_GROUP_EXISTS
    echo "Using existing group: ${GROUP_NAME} (${MY_GID})"
else
    GROUP_NAME=$USER
    addgroup -g "${MY_GID}" "${GROUP_NAME}"
fi

# 2. Create the User (if they don't exist)
# Using '|| true' in case the user was already baked into the image
adduser -u "${MY_UID}" -G "${GROUP_NAME}" -h "/home/${USER}" -D "${USER}" || true

# 3. Fix Permissions
# Use the dynamic group name here
chown -R "${USER}:${GROUP_NAME}" "/home/${USER}"

# 4. Execute
exec su "${USER}" -c "$@"
