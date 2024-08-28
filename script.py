import sqlite3

# Connect to SQLite database
conn = sqlite3.connect('database.db')
cursor = conn.cursor()

# Fetch all players from the players table
cursor.execute("SELECT short_name, club_name FROM players")
players = cursor.fetchall()

cursor.execute("""
CREATE TABLE team_membership (
    team_id INTEGER,
    player_name TEXT,
    PRIMARY KEY(team_id, player_name),
    FOREIGN KEY(player_name) REFERENCES players(short_name),
    FOREIGN KEY(team_id) REFERENCES teams_premier_league(team_id)
)
""")

# Correct creation of the teams_premier_league table
cursor.execute("""
CREATE TABLE teams_premier_league (
    team_id INTEGER PRIMARY KEY AUTOINCREMENT,
    team_name TEXT NOT NULL UNIQUE
)
""")

for short_name, team_name in players:
    if team_name:
        # Insert team into the teams_premier_league table if it doesn't already exist
        cursor.execute("INSERT OR IGNORE INTO teams_premier_league (team_name) VALUES (?)", (team_name,))
        
        # Get the team_id for the current team_name
        cursor.execute("SELECT team_id FROM teams_premier_league WHERE team_name = ?", (team_name,))
        team_id = cursor.fetchone()[0]

        # Check if the player-team relationship already exists
        cursor.execute("SELECT 1 FROM team_membership WHERE team_id = ? AND player_name = ?", (team_id, short_name))
        if cursor.fetchone() is None:
            # Insert the player-team relationship if it doesn't exist
            cursor.execute("INSERT INTO team_membership (team_id, player_name) VALUES (?, ?)", (team_id, short_name))

# Commit the transaction
conn.commit()

# Close the connection
conn.close()
