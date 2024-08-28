function logToFile(message) {
    window.api.logToFile(message);
}

function objectToUint8Array(obj) {
    const str = obj.toString(); // "0,0,0,0,0,...,0"
    const strArray = str.split(',');
    const uint8Array = new Uint8Array(strArray.map(Number));

    return uint8Array;
}

function deserializeStats(buffer) {
    const dataView = new DataView(buffer.buffer);

    const stats = [];
    for (let i = 0; i < buffer.length; i += 4) {
        stats.push(dataView.getInt32(i, true))
    }

    return stats;
}

document.addEventListener('DOMContentLoaded', () => {
    if (!window.api) {
        logToFile('API is not available in the window object.');
        return;
    }

    document.getElementById('connectButton').addEventListener('click', () => {
        window.api.connectToBackend();
    });

    window.api.onConnectionStatus((status) => {
        if (status === 'connected') {
            window.location.href = 'match.html';
        } else {
            document.getElementById('status').textContent = `Status: ${status}`;
        }
    });
});