<template>
  <div>
    <h1>Match Stats</h1>
    <div class="match-stats">
      <table>
        <thead>
        <tr>
          <th>Statistic</th>
          <th>Team A</th>
          <th>Team B</th>
        </tr>
        </thead>
        <tbody>
        <tr>
          <td>Goals</td>
          <td>{{ stats.homeGoals }}</td>
          <td>{{ stats.awayGoals }}</td>
        </tr>
        <tr>
          <td>Possession</td>
          <td>{{ stats.homePossession }}%</td>
          <td>{{ stats.awayPossession }}%</td>
        </tr>
        <tr>
          <td>Shots</td>
          <td>{{ stats.homeShots }}</td>
          <td>{{ stats.awayShots }}</td>
        </tr>
        <tr>
          <td>Shots on Target</td>
          <td>{{ stats.homeShotsOnTarget }}</td>
          <td>{{ stats.awayShotsOnTarget }}</td>
        </tr>
        <tr>
          <td>Passes</td>
          <td>{{ stats.homePasses }}</td>
          <td>{{ stats.awayPasses }}</td>
        </tr>
        </tbody>
      </table>
    </div>
    <button @click="controlMatch('PAUSE')">Pause Match</button>
    <button @click="controlMatch('START')">Start Match</button>
    <button @click="controlMatch('SKIP')">Skip Match</button>
  </div>
</template>

<script>
export default {
  data() {
    return {
      stats: {
        homeGoals: '',
        awayGoals: '',
        homePossession: '',
        awayPossession: '',
        homeShots: '',
        awayShots: '',
        homeShotsOnTarget: '',
        awayShotsOnTarget: '',
        homePasses: '',
        awayPasses: '',
      },
    };
  },
  methods: {
    controlMatch(command) {
      if (window.api) {
        window.api.controlMatch(command);

        if (command === 'SKIP') {
          this.updateStats();
        }
      }
    },
    async updateStats() {
      if (!window.api) return;

      window.api.controlMatch('GET_STATS');
    },
    setMatchData(data) {
      const jsonString = new TextDecoder().decode(data);
      const jsonData = JSON.parse(jsonString);
      let stats = Object.values(jsonData);
      let homePossession = Math.round(stats[7][0] * 100 / (stats[7][0] + stats[7][1]));

      this.stats.homeGoals = stats[0][0];
      this.stats.awayGoals = stats[0][1];
      this.stats.homePossession = homePossession;
      this.stats.awayPossession = 100 - homePossession;
      this.stats.homeShots = stats[1][0];
      this.stats.awayShots = stats[1][1];
      this.stats.homeShotsOnTarget = stats[2][0];
      this.stats.awayShotsOnTarget = stats[2][1];
      this.stats.homePasses = stats[6][0];
      this.stats.awayPasses = stats[6][1];
    },
  },
  mounted() {
    if (!window.api) {
      console.log('API is not available in the window object.');
      return;
    }

    setInterval(this.updateStats, 5000);

    window.api.onUpdateData((data) => {
      this.setMatchData(data);
    });
  },
};
</script>

<style scoped>
.match-stats {
  margin-bottom: 20px;
}

table {
  width: 100%;
  border-collapse: collapse;
}

th, td {
  border: 1px solid #ccc;
  padding: 8px;
  text-align: center;
}

button {
  margin-right: 10px;
  padding: 8px 16px;
}
</style>
