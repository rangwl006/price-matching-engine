#!/bin/bash

echo "START: Start Client Program"

${INSTALL_PATH}/aarch64/bin/tcpClient "${RESOURCE_PATH}/settings.yaml"

echo "START: End Client Program"
