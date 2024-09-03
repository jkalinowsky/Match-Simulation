<template>
  <v-container>
    <v-row justify="center">
      <h1>Match Stats</h1>
    </v-row>
    <v-row justify="end">
      <span>Speed:</span>
      <span @click="selectOption('1X')" :class="{'chosen': selectedOption === '1X'}"
            style="cursor: pointer; margin-right: 10px;">
        1X
        </span>
      <span
          @click="selectOption('3X')" :class="{'chosen': selectedOption === '3X'}"
          style="cursor: pointer; margin-right: 10px;">
          3X
        </span>
      <span
          @click="selectOption('5X')" :class="{'chosen': selectedOption === '5X'}"
          style="cursor: pointer;">
          5X
        </span>
    </v-row>
    <v-container>

      {{ formattedTime }}
      <v-data-table
          :headers="headers"
          :items="items"
          class="elevation-1"
          hide-default-footer
      >
      </v-data-table>
    </v-container>
    <v-container>
      <v-row justify="space-between" align="center">
        <v-col cols="auto">
          <v-btn class="pa-2 ma-2" @click="controlMatch('START')">Resume Match</v-btn>
          <v-btn class="pa-2 ma-2" @click="controlMatch('PAUSE')">Pause Match</v-btn>
        </v-col>
        <v-col cols="auto">
          <v-btn class="pa-2 ma-2" @click="controlMatch('SKIP')">Skip Match</v-btn>
        </v-col>
      </v-row>
    </v-container>
  </v-container>
</template>

<script>
export default {
  computed: {
    headers() {
      return [
        { text: 'Statistic', value: 'statistic', sortable: false },
        { text: localStorage.getItem('teamA'), value: 'firstTeamValue', sortable: false },
        { text: localStorage.getItem('teamB'), value: 'secondTeamValue', sortable: false },
      ]
    },
    items() {
      return [
        { statistic: 'Goals', firstTeamValue: this.stats.homeGoals, secondTeamValue: this.stats.awayGoals },
        { statistic: 'Possession', firstTeamValue: `${this.stats.homePossession}%`, secondTeamValue: `${this.stats.awayPossession}%` },
        { statistic: 'Shots', firstTeamValue: this.stats.homeShots, secondTeamValue: this.stats.awayShots },
        { statistic: 'Shots on Target', firstTeamValue: this.stats.homeShotsOnTarget, secondTeamValue: this.stats.awayShotsOnTarget },
        { statistic: 'Passes', firstTeamValue: this.stats.homePasses, secondTeamValue: this.stats.awayPasses },
      ]
    },
    formattedTime() {
      let minutes = Math.floor(this.stats.time / 60);
      let seconds = this.stats.time % 60;

      if (minutes === 90)
        seconds = 0;

      minutes = minutes < 10 ? `0${minutes}` : `${minutes}`;
      seconds = seconds < 10 ? `0${seconds}` : `${seconds}`;

      return `${minutes}:${seconds}`;
    }
  },
  data() {
    return {
      stats: {
        time: '',
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
      selectedOption: '1X',
    };
  },
  methods: {
    controlMatch(command) {
      if (window.api) {
        window.api.controlMatch(command);
      }
    },
    setMatchData(data) {
      const jsonString = new TextDecoder().decode(data);
      const jsonData = JSON.parse(jsonString);
      let stats = Object.values(jsonData);
      let homePossession = Math.round(stats[8][0] * 100 / (stats[8][0] + stats[8][1]));

      this.stats.time = stats[0][0];
      this.stats.homeGoals = stats[1][0];
      this.stats.awayGoals = stats[1][1];
      this.stats.homePossession = homePossession;
      this.stats.awayPossession = 100 - homePossession;
      this.stats.homeShots = stats[2][0];
      this.stats.awayShots = stats[2][1];
      this.stats.homeShotsOnTarget = stats[3][0];
      this.stats.awayShotsOnTarget = stats[3][1];
      this.stats.homePasses = stats[7][0];
      this.stats.awayPasses = stats[7][1];
    },
    selectOption(option) {
      this.selectedOption = option;
      this.controlMatch(option);
    }
  },
  mounted() {
    if (!window.api) {
      console.log('API is not available in the window object.');
      return;
    }

    window.api.onUpdateData((data) => {
      this.setMatchData(data);
    });
  },
};
</script>

<style scoped>.chosen {
  text-decoration: underline;
  color: blue;
  font-weight: bold;
}
</style>
