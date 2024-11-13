#!/bin/bash

# Check if the script is run with sudo privileges
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root. Try using sudo."
   exit 1
fi

# Check if a port is provided as the first argument
if [[ -z "$1" ]]; then
    echo "Usage: $0 <port> [username:group]"
    exit 1
fi

PORT=$1

# Determine the invoking user and their primary group if username:group is not provided
if [[ -n "$2" ]]; then
    USERNAME=$(echo "$2" | cut -d':' -f1)
    GROUP=$(echo "$2" | cut -d':' -f2)
else
    # Use the SUDO_USER variable to get the non-root user who ran the command with sudo
    USERNAME=${SUDO_USER:-$USER}
    GROUP=$(id -gn "$USERNAME")
fi

# Execute commands to set up authbind permissions
echo "Setting up authbind permissions for port $PORT with user $USERNAME and group $GROUP..."

sudo touch "/etc/authbind/byport/$PORT"
sudo chmod 755 "/etc/authbind/byport/$PORT"
sudo chown "$USERNAME:$GROUP" "/etc/authbind/byport/$PORT"

echo "Authbind permissions set up successfully for user $USERNAME and group $GROUP on port $PORT."
