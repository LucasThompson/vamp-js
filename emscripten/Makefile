all: Vamp.js

Vamp.js:
	em++ -O3 --bind \
	-s ALLOW_MEMORY_GROWTH=1 \
	--memory-init-file 0 \
	-I./ext \
	-I/usr/local/include \
	../src/RawDataAudioStream.cpp \
	../src/SimpleFeatureSetFormatter.cpp \
	../src/VampHost.cpp \
	../src/JsonFeatureSetFormatter.cpp \
	./ext/src/vamp-sdk/FFT.cpp \
	./ext/json11.cpp \
	./ext/src/vamp-sdk/RealTime.cpp \
	./ext/src/vamp-hostsdk/RealTime.cpp \
	./ext/src/vamp-hostsdk/PluginChannelAdapter.cpp \
	./ext/src/vamp-hostsdk/PluginInputDomainAdapter.cpp \
	./ext/src/vamp-hostsdk/PluginWrapper.cpp \
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
	rm -f ./Vamp.js