const net = require('net');
const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const sqlite3 = require('sqlite3').verbose();
const dbPath = path.join(__dirname, '..', 'database.db');


let mainWindow;
let client;
let isConnected = false;

function createWindow() {
    mainWindow = new BrowserWindow({
        width: 1280,
        height: 720,
        webPreferences: {
            preload: path.join(__dirname, 'preload.js'),
            contextIsolation: true,
            enableRemoteModule: false,
            nodeIntegration: false
        }
    });

    mainWindow.maximize();

    mainWindow.loadFile(path.join(__dirname, 'dist', 'index.html'));
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

            client.on('data', (data) => {
                let dataString = data.toString();
                if (dataString.endsWith("ACK")) {
                    dataString = dataString.slice(0, -3); // removing ACK from the end
                }
                try {
                    const parsedData = JSON.parse(dataString);
                    if (parsedData.type === 'match-stats') {
                        console.log('Received match stats');
                        mainWindow.webContents.send('update-data', parsedData);
                    }
                    else if (parsedData.type === 'player-stats') {
                        console.log('Received player stats');
                        mainWindow.webContents.send('player-data', parsedData);
                    }
                }
                catch (error) {
                    console.error('Error parsing received data ', error);
                }
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

            db.all(
                `
                    SELECT team_name
                    FROM teams_premier_league
                    `,
                [], (err, rows) => {
                if (err) {
                    console.error(err.message);
                    reject(err);
                } else {
                    resolve(rows);
                }
                db.close();
            });
        })
    })

    ipcMain.handle('get-players', async(event, team1, team2) => {
        return new Promise((resolve, reject) => {
            const db = new sqlite3.Database(dbPath);

            db.all(`
                      SELECT p.short_name
                      FROM team_membership tm
                      INNER JOIN teams_premier_league tp
                        ON tm.team_id = tp.team_id
                      INNER JOIN players p
                        ON tm.player_name = p.short_name
                      WHERE (tp.team_name = ? OR tp.team_name = ?)
                      AND p.is_first_squad = 'True'
                      ORDER BY 
                        CASE 
                            WHEN tp.team_name = ? THEN 1
                            WHEN tp.team_name = ? THEN 2
                        END,
                      p.first_position ASC;
                    `,
                [team1, team2, team1, team2], (err, rows) => {
                if (err) {
                    console.error(err.message);
                    reject(err);
                }else {
                    resolve(rows);
                }
            });

            db.close();
        })
    })

    ipcMain.handle('get-player', async(event, player) => {
        return new Promise((resolve, reject) => {
            const db = new sqlite3.Database(dbPath);

            db.all(`
                        SELECT age, nationality_name, pace, attacking_finishing, power_long_shots,
                         dribbling, defending, attacking_short_passing, skill_long_passing, mentality_vision,
                        goalkeeping_diving, goalkeeping_handling, goalkeeping_kicking, goalkeeping_positioning,
                        goalkeeping_reflexes, first_position, second_position
                        FROM players
                        WHERE short_name = ?
            `,
            [player], (err, rows) => {
                if (err) {
                    console.error(err.message);
                    reject(err);
                }
                else {
                    resolve(rows[0]);
                }
                });

            db.close();
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