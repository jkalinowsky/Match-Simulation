const net = require('net');
const { app, BrowserWindow, ipcMain } = require('electron');
const fs = require('fs');
const path = require('path');
const sqlite3 = require('sqlite3').verbose();
const dbPath = path.join(__dirname, '..', 'database.db');

let mainWindow;
let client;
let isConnected = false;

const logFilePath = path.join(__dirname, 'log.txt');

function logToFile(message) {
    const timestamp = new Date().toISOString();
    fs.appendFile(logFilePath, `${timestamp} - ${message}\n`, (err) => {
        if (err) console.error('Failed to write to log file:', err);
    });
}

function createWindow() {
    mainWindow = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            preload: path.join(__dirname, 'preload.js'),
            contextIsolation: true,
            enableRemoteModule: false,
            nodeIntegration: false
        }
    });

    mainWindow.loadFile('index.html')
}

app.whenReady().then(() => {
    createWindow();

    ipcMain.on('connect-to-backend', (event, team1, team2) => {
        if (!isConnected) {
            client = new net.Socket();
            client.connect(27015, '127.0.0.1', () => {
                isConnected = true;
                mainWindow.webContents.send('connection-status', 'connected');

                const data = JSON.stringify({ team1, team2 });
                client.write(data + '\n');
            });

            ipcMain.on('control-match', (event, command) => {
                client.write(command);
            });

            ipcMain.on('log-message', (event, message) => {
                logToFile(message);
            });

            client.on('data', (data) => {
                mainWindow.webContents.send('update-data', data);
            });

            client.on('error', (err) => {
                console.error('Socket error:', err);
            });

            client.on('close', () => {
                console.log('Socket closed');
            });
        }
    });

    ipcMain.handle('get-teams', async () => {
        return new Promise((resolve, reject) => {
            const db = new sqlite3.Database(dbPath);

            db.all('SELECT team_name FROM teams_premier_league', [], (err, rows) => {
                if (err) {
                    reject(err);
                } else {
                    resolve(rows);
                }
                db.close();
            });
        })
    })
});

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit();
    }
});

app.on('activate', () => {
    if (mainWindow === null) {
        createWindow();
    }
});