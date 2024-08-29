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

document.addEventListener('DOMContentLoaded', async () => {
    if (!window.api) {
        logToFile('API is not available in the window object.');
        return;
    }

    const rows = await window.api.getTeams();

    const team1Select = document.getElementById('team1');
    const team2Select = document.getElementById('team2');

    rows.forEach(row => {
        // Create separate option elements for each select
        const option = document.createElement('option');
        option.value = row.team_name; // Use the actual team_name
        option.textContent = row.team_name;

        // Append the options to both selects
        team1Select.appendChild(option.cloneNode(true)); // Clone for the second select
        team2Select.appendChild(option);
    });

    document.getElementById('connectButton').addEventListener('click', () => {
        const team1 = document.getElementById('team1').value;
        const team2 = document.getElementById('team2').value;

        if (!team1 || !team2 && team1 === team2) {
            alert('Please select both teams!');
            return;
        }

        window.api.connectToBackend(team1, team2);
    });

    window.api.onConnectionStatus((status) => {
        if (status === 'connected') {
            window.location.href = 'match.html';
        } else {
            document.getElementById('status').textContent = `Status: ${status}`;
        }
    });
});