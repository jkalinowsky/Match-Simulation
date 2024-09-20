const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('api', {
    controlMatch: (command) => ipcRenderer.send('control-match', command),
    onUpdateData: (callback) => ipcRenderer.on('update-data', (event, data) => callback(data)),
    onPlayerData: (callback) => ipcRenderer.on('player-data', (event, data) => callback(data)),
    connectToBackend: (team1, team2) => ipcRenderer.send('connect-to-backend', team1, team2),
    getTeams: () => ipcRenderer.invoke('get-teams'),
    getPlayers: (team1, team2) => ipcRenderer.invoke('get-players', team1, team2),
    getPlayer: (player) => ipcRenderer.invoke('get-player', player),
});