var Module = {
    canvas: document.getElementById('gameCanvas'),
    context: null,
    imageData: null,
    keyState: 0,

    onRuntimeInitialized: function() {
        // console.log('WebAssembly module loaded');
        this.context = this.canvas.getContext('2d');
        this.imageData = this.context.createImageData(128, 32);
        this.addEventListeners();
        this.gameLoop();
        this.setupUserIsr(); 
    },

    setupUserIsr: function() {
        // console.log("Setting up interval for user_isr");
        this.userIsrInterval = setInterval(() => {
            if (Module.ccall) {
                Module.ccall('user_isr', null, [], []);
            }
        }, 10); // Call every 10ms
    },
    

    addEventListeners: function() {
        let keyRepeatTimer = null;
        const keyRepeatDelay = 200; // ms
    
        const handleKeyPress = (event) => {
            switch(event.key.toLowerCase()) {
                case 'arrowleft':
                    this.keyState |= 4;  // Player 1 left
                    break;
                case 'arrowright':
                    this.keyState |= 2;  // Player 1 right
                    break;
                case 'a':
                    this.keyState |= 8;  // Player 2 left (new bit)
                    break;
                case 'd':
                    this.keyState |= 16; // Player 2 right (new bit)
                    break;
                case 'enter':
                case ' ':
                    this.keyState |= 1;  // Start/select
                    break;
            }
    
            if (keyRepeatTimer === null) {
                keyRepeatTimer = setInterval(() => {
                    console.log('Key repeat, keyState:', this.keyState);
                    Module.ccall('labwork', null, [], []);
                }, keyRepeatDelay);
            }
        };
    
        const handleKeyRelease = (event) => {
            switch(event.key.toLowerCase()) {
                case 'arrowleft':
                    this.keyState &= ~4;
                    break;
                case 'arrowright':
                    this.keyState &= ~2;
                    break;
                case 'a':
                    this.keyState &= ~8;
                    break;
                case 'd':
                    this.keyState &= ~16;
                    break;
                case 'enter':
                case ' ':
                    this.keyState &= ~1;
                    break;
            }
    
            clearInterval(keyRepeatTimer);
            keyRepeatTimer = null;
        };
    
        document.addEventListener('keydown', handleKeyPress);
        document.addEventListener('keyup', handleKeyRelease);
    },

    lastUpdateTime: 0,
    updateInterval: 60,
    
    gameLoop: function(timestamp) {
        if (!this.lastUpdateTime) this.lastUpdateTime = timestamp;
    
        const elapsed = timestamp - this.lastUpdateTime;
    
        if (elapsed > this.updateInterval) {
            if (Module.ccall) {
                Module.ccall('labwork', null, [], []);
                Module.ccall('display_update', null, [], []);
            } else {
                // console.log("Module.ccall not available");
            }
    
            this.lastUpdateTime = timestamp;
        }
    
        requestAnimationFrame(this.gameLoop.bind(this));
    },

    updateDisplay: function(bufferPtr, bufferSize) {
        var buffer = new Uint8Array(Module.HEAPU8.buffer, bufferPtr, bufferSize);
        for (var x = 0; x < 128; x++) {
            for (var byteY = 0; byteY < 4; byteY++) {
                var byte = buffer[x + byteY * 128];
                for (var bit = 0; bit < 8; bit++) {
                    var pixelOn = (byte & (1 << bit)) !== 0;
                    var y = byteY * 8 + bit;
                    var index = (y * 128 + x) * 4;
                    var color = pixelOn ? 255 : 0;
                    this.imageData.data[index] = color;
                    this.imageData.data[index + 1] = color;
                    this.imageData.data[index + 2] = color;
                    this.imageData.data[index + 3] = 255;
                }
            }
        }
        this.context.putImageData(this.imageData, 0, 0);
    },

    getButtons: function() {
        return this.keyState;
    }
    
    
};

