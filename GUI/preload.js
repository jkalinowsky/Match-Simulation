const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('api', {
    controlMatch: (command) => ipcRenderer.send('control-match', command),
    onUpdateData: (callback) => ipcRenderer.on('update-data', (event, data) => callback(data)),
    logToFile: (message) => ipcRenderer.send('log-message', message),
    connectToBackend: () => ipcRenderer.send('connect-to-backend'),
    onConnectionStatus: (callback) => ipcRenderer.on('connection-status', (event, status) => callback(status))
});