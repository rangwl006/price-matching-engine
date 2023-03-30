#!/bin/bash

echo "START: Start tcp server"
${INSTALL_PATH}/aarch64/bin/tcpServer "${RESOURCE_PATH}/settings.yaml"
echo "END: End tcp server"