<template>
  <v-container>
    <h1>Welcome to v. 1.0 of OpenFm match simulation!</h1>
    <p>Here u can select your favorite team and simulate match. Actually u can't change players or tactic.</p>
  </v-container>
  <v-container>
    <h3>Choose team to simulation</h3>
    <label for="team1">Team 1:</label>
    <v-select
        id="team1" label="Select first team" v-model="selectedTeam1" :items="teamNames" variant="solo-filled"
    ></v-select>

    <label for="team2">Team 2:</label>
    <v-select
        id="team2" label="Select second team" v-model="selectedTeam2" :items="teamNames" variant="solo-filled"
    ></v-select>

    <v-btn @click="startMatch">
      Start match
    </v-btn>
  </v-container>
</template>

<script>

export default {
  computed: {
    teamNames() {
      return this.teams.map(team => team.team_name);
    }
  },
  data() {
    return {
      teams: [],
      selectedTeam1: '',
      selectedTeam2: '',
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
  },
  methods: {
    startMatch() {
      if (!this.selectedTeam1 || !this.selectedTeam2 || this.selectedTeam1 === this.selectedTeam2) {
        alert('Please select both teams!');
        return;
      }
      localStorage.setItem('teamA', this.selectedTeam1);
      localStorage.setItem('teamB', this.selectedTeam2);
      this.$router.push({ path: '/match'});
      window.location.href='../dist/match.html';
      window.api.connectToBackend(this.selectedTeam1, this.selectedTeam2);
    }
  }
};
</script>

<style scoped>
</style>
