// define online and offline audio context

var audioCtx = new AudioContext();
var offlineCtx = new OfflineAudioContext(2, 44100 * 40, 44100);

source = offlineCtx.createBufferSource();

// define variables

var play = document.querySelector('.play'); 

// use XHR to load an audio track, and
// decodeAudioData to decode it and stick it in a buffer.
// Then we put the buffer into the source

function getData() {
  request = new XMLHttpRequest();

  request.open('GET', 'viper.ogg', true);

  request.responseType = 'arraybuffer';


  request.onload = function() {
    var audioData = request.response;

    audioCtx.decodeAudioData(audioData, function(buffer) {
      myBuffer = buffer;
      source.buffer = myBuffer;
      source.connect(offlineCtx.destination);
      source.start();
      offlineCtx.startRendering().then(function(renderedBuffer) {
        console.log('Rendering completed successfully');
        console.log('Vamp stuff happening..');
        console.log('Length: ' + renderedBuffer.length);
        console.log('nChannels: ' + renderedBuffer.numberOfChannels);
        console.log('sampleRate: ' + renderedBuffer.sampleRate);
        //
        var as = Module.createRawDataAudioStream(renderedBuffer.length, renderedBuffer.numberOfChannels, renderedBuffer.sampleRate);
        for (var c = 0; c < renderedBuffer.numberOfChannels; ++c) {
            var cBuffer = renderedBuffer.getChannelData(c);
            for (var n = 0; n < renderedBuffer.length; ++n) {
                as.setSample(n, c, cBuffer[n]);
            } 
        }
        //
        var host = new Module.VampHost(Module.createPYin(renderedBuffer.sampleRate));
        console.log(host.run(as, Module.createSimpleFeatureSetFormatter(5)));
        
        // clean up emscripten objects
        host.delete(); 
        
        console.log('Vamp stuff done..');
        document.querySelector('#status').innerHTML = 'Vamp stuff done..'
        var audioCtx = new (window.AudioContext || window.webkitAudioContext)(); 
        var song = audioCtx.createBufferSource();
        song.buffer = renderedBuffer;
        song.connect(audioCtx.destination);

        play.onclick = function() {
          song.start();
        }
      }).catch(function(err) {
          console.log('Rendering failed: ' + err);
          // Note: The promise should reject when startRendering is called a second time on an OfflineAudioContext
      });
    });
  }

  request.send();
}

// Run getData to start the process off

getData();