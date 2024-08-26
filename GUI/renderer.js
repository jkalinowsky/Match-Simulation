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

    document.getElementById('pause-btn')?.addEventListener('click', () => {
        window.api.controlMatch('PAUSE');
    });

    document.getElementById('start-btn')?.addEventListener('click', () => {
        window.api.controlMatch('START');
    });

    document.getElementById('skip-btn')?.addEventListener('click', () => {
        window.api.controlMatch('SKIP');
    });

    setInterval(() => {
        if (window.api) {
            window.api.controlMatch('GET_STATS');
        }
    }, 5000);

    window.api.onUpdateData((data) => {
        const uint8Array = objectToUint8Array(data);
        const stats = deserializeStats(uint8Array);

        let homePos = Math.round(stats[14] / (stats[14] + stats[15]))

        document.getElementById("homeGoals").innerHTML = stats[0];
        document.getElementById("awayGoals").innerHTML = stats[1];
        document.getElementById("homePos").innerHTML = homePos + "%";
        document.getElementById("awayPos").innerHTML = (100-homePos) + "%";
        document.getElementById("homeShots").innerHTML = stats[2];
        document.getElementById("awayShots").innerHTML = stats[3];
        document.getElementById("homeShotsOT").innerHTML = stats[4];
        document.getElementById("awayShotsOT").innerHTML = stats[5];
        document.getElementById("homePasses").innerHTML = stats[12];
        document.getElementById("awayPasses").innerHTML = stats[13];
    });
});