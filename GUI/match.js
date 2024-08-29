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
        window.api.controlMatch('GET_STATS');
    });

    setInterval(() => {
        if (window.api) {
            window.api.controlMatch('GET_STATS');
        }
    }, 5000);

    window.api.onUpdateData((data) => {
        // statystyki nie dzialaja?
        let stats;
        stats = JSON.parse(data);

        let homePos = Math.round(stats[14] * 100 / (stats[14] * 100 + stats[15] * 100) * 100);

        document.getElementById("homeGoals").innerHTML = stats[0];
        document.getElementById("awayGoals").innerHTML = stats[1];
        document.getElementById("homePos").innerHTML = homePos + "%";
        document.getElementById("awayPos").innerHTML = (100 - homePos) + "%";
        document.getElementById("homeShots").innerHTML = stats[2];
        document.getElementById("awayShots").innerHTML = stats[3];
        document.getElementById("homeShotsOT").innerHTML = stats[4];
        document.getElementById("awayShotsOT").innerHTML = stats[5];
        document.getElementById("homePasses").innerHTML = stats[12];
        document.getElementById("awayPasses").innerHTML = stats[13];
    });
});