const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('api', {
    controlMatch: (command) => ipcRenderer.send('control-match', command),
    onUpdateData: (callback) => ipcRenderer.on('update-data', (event, data) => callback(data)),
    logToFile: (message) => ipcRenderer.send('log-message', message)
});