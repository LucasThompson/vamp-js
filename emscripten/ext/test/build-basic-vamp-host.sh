#!/usr/bin/env bash
{
make -C ../../ SubModules
cd ../vamp-plugin-sdk
./configure
make host
cd ../test
}	&> /dev/null
diff expected-fixed-tempo-detection-function.txt <(../vamp-plugin-sdk/host/vamp-simple-host vamp-example-plugins:fixedtempo:detectionfunction ../../../test/fixtures/test.wav)
