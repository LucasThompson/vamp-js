const vamp = require('../Vamp.js');
var sineData = require('./SineData.js');

var sampleRate = 8000.0;
var as = vamp.createRawDataAudioStream(sineData.length, 1, sampleRate);

for (var i = 0; i < sineData.length; ++i) {
    as.setSample(i, 0, sineData[i]);
}

var host = new vamp.VampHost(vamp.createPowerSpectrum(sampleRate));
console.log(host.run(as, vamp.createSimpleFeatureSetFormatter(0)));
host.delete();

