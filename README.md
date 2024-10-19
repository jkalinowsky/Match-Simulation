# Match Simulation

This repository contains a **Match Simulation** project, which is divided into two main parts: the **backend** (C++) and the **frontend** (Vue.js with Electron).

## Project Overview
Originally, this match simulation system was part of a larger concept for a football manager game. Although the direction of the football manager game has changed, the core mechanics of this match simulation remain and will play a crucial role in the full project, which is still under development.

### 1. Backend: C++ Match Simulation
The backend is written in C++ and handles the match simulation logic. It utilizes websockets for two-way communication with the frontend. 

- **Decision Trees and match mechanics**: Players' actions and decisions are based on a combination of attributes and randomness. A decision tree helps simulate real-life football decisions, such as passing, shooting, or positioning. A player's movement is based on their position and the opponent's decisions.
- **Multithreading**: Threads are used to manage communication between the backend and the frontend, ensuring that the simulation can be paused, resumed, or stopped while sending and receiving real-time data.
- **Websockets**: These are essential for controlling the simulation flow, sending player statistics and match data to the frontend, and handling commands like stopping the simulation or skipping to the end.

### 2. Frontend: Vue.js & Electron
The frontend is built using Vue.js and Electron, offering a desktop application where users can interact with the match simulation.

- **Team Selection**: Users can select two teams from the English Premier League.
- **Match Control**: The interface allows control over the simulation, including starting, pausing, resuming, and skipping to the end of a match.
- **Player & Match Statistics**: The app provides detailed statistics about players and the overall match, updating in real time as the simulation progresses.
- **Database**: The player and team data are derived from FIFA 23, though modified to suit the simulation's specific needs.

## Future Development
This simulation is part of a larger football manager project that is currently under development. While the scope of the original project has changed, the simulation mechanics developed here will be used in new ways as the football manager game evolves.

Stay tuned for more updates as the project grows!

## License
This project is licensed under the MIT License.
