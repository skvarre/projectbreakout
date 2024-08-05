var Module = {
    onRuntimeInitialized: function() {
        console.log("WebAssembly module initialized");
    }
};

let buttonState = 0;

Module.getButtons = function() {
    return buttonState;
};

Module.updateDisplay = function() {
    const displayBufferPtr = Module._get_display_buffer_ptr();
    const displayBuffer = new Uint8Array(Module.HEAPU8.buffer, displayBufferPtr, 512);
    const canvas = document.getElementById('gameCanvas');
    const ctx = canvas.getContext('2d');

    ctx.fillStyle = 'black';
    ctx.fillRect(0, 0, 128, 32);
    ctx.fillStyle = 'white';

    for (let y = 0; y < 32; y++) {
        for (let x = 0; x < 128; x++) {
            if (displayBuffer[y * 16 + Math.floor(x / 8)] & (1 << (7 - (x % 8)))) {
                ctx.fillRect(x, y, 1, 1);
            }
        }
    }
};

function gameLoop() {
    Module._labwork();
    requestAnimationFrame(gameLoop);
}

document.addEventListener('keydown', function(event) {
    switch(event.key) {
        case 'ArrowLeft': buttonState |= 1; break;
        case 'ArrowRight': buttonState |= 2; break;
        case 'ArrowUp': buttonState |= 4; break;
    }
});

document.addEventListener('keyup', function(event) {
    switch(event.key) {
        case 'ArrowLeft': buttonState &= ~1; break;
        case 'ArrowRight': buttonState &= ~2; break;
        case 'ArrowUp': buttonState &= ~4; break;
    }
});