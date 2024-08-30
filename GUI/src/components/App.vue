<template>
  <div>
    <h1>Choose team to simulation</h1>

    <label for="team1">Team 1:</label>
    <select id="team1" v-model="selectedTeam1">
      <option v-for="team in teams" :key="team.team_name" :value="team.team_name">
        {{ team.team_name }}
      </option>
    </select>

    <label for="team2">Team 2:</label>
    <select id="team2" v-model="selectedTeam2">
      <option v-for="team in teams" :key="team.team_name" :value="team.team_name">
        {{ team.team_name }}
      </option>
    </select>

    <router-link to="/match" @click="startMatch">Start match</router-link>
    <p id="status">Status: {{ connectionStatus }}</p>
  </div>
</template>

<script>

export default {
  data() {
    return {
      teams: [],
      selectedTeam1: '',
      selectedTeam2: '',
      connectionStatus: 'Disconnected',
    };
  },
  async mounted() {
    if (!window.api) {
      console.log('API is not available in the window object.');
      return;
    }

    try {
      this.teams = await window.api.getTeams();
    } catch (error) {
      console.log('Failed to fetch teams:', error);
    }

    window.api.onConnectionStatus((status) => {
      this.connectionStatus = status;
    });
  },
  methods: {
    startMatch() {
      if (!this.selectedTeam1 || !this.selectedTeam2 || this.selectedTeam1 === this.selectedTeam2) {
        alert('Please select both teams!');
        return;
      }
      window.location.href='../dist/match.html';
      window.api.connectToBackend(this.selectedTeam1, this.selectedTeam2);
    }
  }
};
</script>

<style scoped>
</style>
