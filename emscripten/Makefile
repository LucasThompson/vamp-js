all: SubModules Vamp.js

SubModules:
	if [ -d ./plugins/pyin/.hg ]; then \
		hg pull -u -R ./plugins/pyin; \
	else \
		hg clone https://code.soundsoftware.ac.uk/hg/pyin -r vamp-fft-revision ./plugins/pyin; \
	fi
	if [ -d ./ext/vamp-plugin-sdk/.hg ]; then \
		hg pull -u -R ./ext/vamp-plugin-sdk; \
	else \
		hg clone https://code.soundsoftware.ac.uk/hg/vamp-plugin-sdk -r vampipe ./ext/vamp-plugin-sdk; \
	fi

Vamp.js:
	em++ -O3 -DSINGLE_PRECISION_FFT --bind \
	-s ALLOW_MEMORY_GROWTH=1 \
	--memory-init-file 0 \
	-I./ext \
	-I./ext/vamp-plugin-sdk \
	-I/usr/local/include \
	../src/RawDataAudioStream.cpp \
	../src/SimpleFeatureSetFormatter.cpp \
	../src/VampHost.cpp \
	../src/JsonFeatureSetFormatter.cpp \
	./ext/json11.cpp \
	./ext/vamp-plugin-sdk/src/vamp-sdk/FFT.cpp \
	./ext/vamp-plugin-sdk/src/vamp-sdk/RealTime.cpp \
	./ext/vamp-plugin-sdk/src/vamp-hostsdk/RealTime.cpp \
	./ext/vamp-plugin-sdk/src/vamp-hostsdk/PluginChannelAdapter.cpp \
	./ext/vamp-plugin-sdk/src/vamp-hostsdk/PluginInputDomainAdapter.cpp \
	./ext/vamp-plugin-sdk/src/vamp-hostsdk/PluginWrapper.cpp \
	./plugins/ZeroCrossing.cpp \
	./plugins/PowerSpectrum.cpp \
	./plugins/FixedTempoEstimator.cpp \
	./plugins/pyin/PYinVamp.cpp \
	./plugins/pyin/Yin.cpp \
	./plugins/pyin/MonoPitch.cpp \
	./plugins/pyin/MonoNote.cpp \
	./plugins/pyin/MonoPitchHMM.cpp \
	./plugins/pyin/MonoNoteParameters.cpp \
	./plugins/pyin/MonoNoteHMM.cpp \
	./plugins/pyin/YinUtil.cpp \
	./plugins/pyin/SparseHMM.cpp \
	VampJsBindings.cpp \
	-o Vamp.js
	
clean:
	rm -rf ./Vamp.js ./plugins/pyin ./ext/vamp-plugin-sdk