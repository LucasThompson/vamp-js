// define  audio context
var AudioContext = window.AudioContext || window.webkitAudioContext;
var audioCtx = new AudioContext();

// define variables
var play = document.querySelector('.play');

// use XHR to load an audio track, and
// decodeAudioData to decode it and stick it in a buffer.
function vampExample(audioFileURI, pluginInitCallback, postProcessingCallback) {
  request = new XMLHttpRequest();

  request.open('GET', audioFileURI, true);

  request.responseType = 'arraybuffer';

  request.onload = function() {
    var audioData = request.response;
    audioCtx.decodeAudioData(audioData).then(function(renderedBuffer) {
      var as = Module.createRawDataAudioStream(renderedBuffer.length, renderedBuffer.numberOfChannels, renderedBuffer.sampleRate);
      for (var c = 0; c < renderedBuffer.numberOfChannels; ++c) {
        var cBuffer = renderedBuffer.getChannelData(c);
        for (var n = 0; n < renderedBuffer.length; ++n) {
          as.setSample(n, c, cBuffer[n]);
        }
      }
      var host = pluginInitCallback(renderedBuffer.sampleRate);
      var feature = JSON.parse(host.run(as, Module.createJsonFeatureSetFormatter(0)));
      host.delete(); // clean up emscripten objects
      document.querySelector('#status').innerHTML = 'Vamp feature extraction done..'
      var song = audioCtx.createBufferSource();
      song.buffer = renderedBuffer;
      song.connect(audioCtx.destination);

      play.onclick = function() {
        song.start();
      }
      
      postProcessingCallback(feature.feature[0].data[0]);
    }).catch(function(err) {
      console.log('Rendering failed: ' + err);
    });
  }
  request.send();
}