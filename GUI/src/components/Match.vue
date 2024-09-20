<template>
  <v-container>
    <v-row>
      <v-col cols="2" class="ml-n1">
        <v-container>
          <span>Home players</span>
          <v-row class="players">
            <v-col v-for="(player, index) in players.slice(0, 11)" :key="index">
              <v-dialog max-width="700">
                <template v-slot:activator="{ props: activatorProps }">
                  <v-btn
                      v-bind="activatorProps"
                      color="surface-variant"
                      text="Open Dialog"
                      variant="flat"
                      @click="openDialog(player.short_name)"
                      block
                  >
                    {{ player.short_name }}
                  </v-btn>
                </template>

                <template v-slot:default="{ isActive }">
                  <v-card title="Player info">
                    <v-card-text>
                      <v-container class="bg-indigo-lighten-2">
                        <v-row justify="space-between">
                          <v-col>
                            <h2>{{player.short_name}}</h2>
                          </v-col>
                          <v-col class="text-right">
                            <h2>{{this.playerAttributes.nationality_name}}</h2>
                          </v-col>
                        </v-row>
                        <v-row justify="space-between">
                          <v-col>
                            <h3>POS: {{this.playerAttributes.first_position}}{{this.playerAttributes.second_position}}</h3>
                          </v-col>
                          <v-col class="text-right">
                            <h3>age {{this.playerAttributes.age}}</h3>
                          </v-col>
                        </v-row>
                        <v-row>
                          <v-col>
                            <h3>APP: 8(1)</h3>
                            <h3>G: 3  A: 1</h3>
                          </v-col>
                        </v-row>
                      </v-container>
                      <v-container class="bg-indigo-lighten-2 mt-3">
                        <v-row>
                          <h3 class="text-left">ATTRIBUTES</h3>
                        </v-row>
                        <v-row>
                          <v-col>
                            <v-row v-for="(value, attribute) in firstColumnAttributes" :key="attribute" justify="space-between">
                              <v-col>
                                <span>{{ attribute.charAt(0).toUpperCase() + attribute.slice(1).replace(/_/g, ' ') }}</span>
                              </v-col>
                              <v-col class="text-right">
                                <span :class="getColorClass(value)">{{ value }}</span>
                              </v-col>
                            </v-row>
                          </v-col>

                          <v-col>
                            <v-row v-for="(value, attribute) in secondColumnAttributes" :key="attribute" justify="space-between">
                              <v-col>
                                <span>{{ attribute.charAt(0).toUpperCase() + attribute.slice(1).replace(/_/g, ' ') }}</span>
                              </v-col>
                              <v-col class="text-right">
                                <span :class="getColorClass(value)">{{ value }}</span>
                              </v-col>
                            </v-row>
                          </v-col>
                        </v-row>
                      </v-container>
                      <v-container class="bg-indigo-lighten-2 mt-3">
                        <v-row>
                          <h3 class="text-left">MATCH STATS</h3>
                        </v-row>
                        <v-row>
                          <v-col>
                            <v-row v-for="(value, stat) in firstColumnStats" :key="stat" justify="space-between">
                              <v-col>
                                <span>{{ stat.charAt(0).toUpperCase() + stat.slice(1).replace(/_/g, ' ') }}</span>
                              </v-col>
                              <v-col class="text-right">
                                <span>{{ value }}</span>
                              </v-col>
                            </v-row>
                          </v-col>

                          <v-col>
                            <v-row v-for="(value, stat) in secondColumnStats" :key="stat" justify="space-between">
                              <v-col>
                                <span>{{ stat.charAt(0).toUpperCase() + stat.slice(1).replace(/_/g, ' ') }}</span>
                              </v-col>
                              <v-col class="text-right">
                                <span>{{ value }}</span>
                              </v-col>
                            </v-row>
                          </v-col>

                          <v-col>
                            <v-row v-for="(value, stat) in thirdColumnStats" :key="stat" justify="space-between">
                              <v-col>
                                <span>{{ stat.charAt(0).toUpperCase() + stat.slice(1).replace(/_/g, ' ') }}</span>
                              </v-col>
                              <v-col class="text-right">
                                <span>{{ value }}</span>
                              </v-col>
                            </v-row>
                          </v-col>
                        </v-row>
                      </v-container>
                    </v-card-text>

                    <v-card-actions>
                      <v-spacer></v-spacer>

                      <v-btn
                          text="Close Dialog"
                          @click="isActive.value = false"
                      ></v-btn>
                    </v-card-actions>
                  </v-card>
                </template>
              </v-dialog>

            </v-col>
          </v-row>
        </v-container>
      </v-col>

      <v-col cols="8">
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
      </v-col>

      <v-col cols="2" class="mr-n1">
        <v-container>
          <span>Away players</span>

          <v-row class="players">
            <v-col v-for="(player, index) in players.slice(11, 22)" :key="index">
              <v-dialog max-width="700">
                <template v-slot:activator="{ props: activatorProps }">
                  <v-btn
                      v-bind="activatorProps"
                      color="surface-variant"
                      text="Open Dialog"
                      variant="flat"
                      @click="openDialog(player.short_name)"
                      block
                  >
                    {{ player.short_name }}
                  </v-btn>
                </template>

                <template v-slot:default="{ isActive }">
                  <v-card title="Player info">
                    <v-card-text>
                      <v-container class="bg-indigo-lighten-2">
                        <v-row justify="space-between">
                          <v-col>
                            <h2>{{player.short_name}}</h2>
                          </v-col>
                          <v-col class="text-right">
                            <h2>{{this.playerAttributes.nationality_name}}</h2>
                          </v-col>
                        </v-row>
                        <v-row justify="space-between">
                          <v-col>
                            <h3>POS: {{this.playerAttributes.first_position}}{{this.playerAttributes.second_position}}</h3>
                          </v-col>
                          <v-col class="text-right">
                            <h3>age {{this.playerAttributes.age}}</h3>
                          </v-col>
                        </v-row>
                        <v-row>
                          <v-col>
                            <h3>APP: 8(1)</h3>
                            <h3>G: 3  A: 1</h3>
                          </v-col>
                        </v-row>
                      </v-container>
                      <v-container class="bg-indigo-lighten-2 mt-3">
                        <v-row>
                          <h3 class="text-left">ATTRIBUTES</h3>
                        </v-row>
                        <v-row>
                          <v-col>
                            <v-row v-for="(value, attribute) in firstColumnAttributes" :key="attribute" justify="space-between">
                              <v-col>
                                <span>{{ attribute.charAt(0).toUpperCase() + attribute.slice(1).replace(/_/g, ' ') }}</span>
                              </v-col>
                              <v-col class="text-right">
                                <span :class="getColorClass(value)">{{ value }}</span>
                              </v-col>
                            </v-row>
                          </v-col>

                          <v-col>
                            <v-row v-for="(value, attribute) in secondColumnAttributes" :key="attribute" justify="space-between">
                              <v-col>
                                <span>{{ attribute.charAt(0).toUpperCase() + attribute.slice(1).replace(/_/g, ' ') }}</span>
                              </v-col>
                              <v-col class="text-right">
                                <span :class="getColorClass(value)">{{ value }}</span>
                              </v-col>
                            </v-row>
                          </v-col>
                        </v-row>
                      </v-container>
                      <v-container class="bg-indigo-lighten-2 mt-3">
                        <v-row>
                          <h3 class="text-left">MATCH STATS</h3>
                        </v-row>
                        <v-row>
                          <v-col>
                            <v-row v-for="(value, stat) in firstColumnStats" :key="stat" justify="space-between">
                              <v-col>
                                <span>{{ stat.charAt(0).toUpperCase() + stat.slice(1).replace(/_/g, ' ') }}</span>
                              </v-col>
                              <v-col class="text-right">
                                <span>{{ value }}</span>
                              </v-col>
                            </v-row>
                          </v-col>

                          <v-col>
                            <v-row v-for="(value, stat) in secondColumnStats" :key="stat" justify="space-between">
                              <v-col>
                                <span>{{ stat.charAt(0).toUpperCase() + stat.slice(1).replace(/_/g, ' ') }}</span>
                              </v-col>
                              <v-col class="text-right">
                                <span>{{ value }}</span>
                              </v-col>
                            </v-row>
                          </v-col>

                          <v-col>
                            <v-row v-for="(value, stat) in thirdColumnStats" :key="stat" justify="space-between">
                              <v-col>
                                <span>{{ stat.charAt(0).toUpperCase() + stat.slice(1).replace(/_/g, ' ') }}</span>
                              </v-col>
                              <v-col class="text-right">
                                <span>{{ value }}</span>
                              </v-col>
                            </v-row>
                          </v-col>
                        </v-row>
                      </v-container>
                    </v-card-text>

                    <v-card-actions>
                      <v-spacer></v-spacer>

                      <v-btn
                          text="Close Dialog"
                          @click="isActive.value = false"
                      ></v-btn>
                    </v-card-actions>
                  </v-card>
                </template>
              </v-dialog>

            </v-col>
          </v-row>
        </v-container>
      </v-col>
    </v-row>

    <v-row>
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
    </v-row>
  </v-container>
</template>

<script>
export default {
  computed: {
    homeTeam() {
      return localStorage.getItem('teamA');
    },
    awayTeam() {
      return localStorage.getItem('teamB');
    },
    headers() {
      return [
        { text: 'Statistic', value: 'statistic', sortable: false },
        { text: this.homeTeam, value: 'firstTeamValue', sortable: false },
        { text: this.awayTeam, value: 'secondTeamValue', sortable: false },
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
    },
    firstColumnAttributes() {
      return {
        pace: this.playerAttributes.pace,
        finishing: this.playerAttributes.attacking_finishing,
        longShots: this.playerAttributes.power_long_shots,
        dribbling: this.playerAttributes.dribbling
      };
    },
    secondColumnAttributes() {
      return {
        defending: this.playerAttributes.defending,
        passing: this.playerAttributes.attacking_short_passing,
        longPassing: this.playerAttributes.skill_long_passing,
        vision: this.playerAttributes.mentality_vision
      };
    },
    firstColumnStats() {
      return {
        goals: this.playerStats.goals,
        assists: this.playerStats.assists,
        touches: this.playerStats.touches,
        interceptions: this.playerStats.interceptions,
        blocks: this.playerStats.blocks
      }
    },
    secondColumnStats() {
      return {
        passesAttempt: this.playerStats.passesAttempt,
        passesCompleted: this.playerStats.passesCompleted,
        shortPasses: this.playerStats.shortPasses,
        longPasses: this.playerStats.longPasses,
        throughPasses: this.playerStats.throughPasses,
      }
    },
    thirdColumnStats() {
      return {
        shotsAttempt: this.playerStats.shots,
        shotsOnTarget: this.playerStats.shotsOT,
        dribblesAttempt: this.playerStats.dribblesAttempt,
        dribblesCompleted: this.playerStats.dribblesCompleted,
      }
    }
  },
  data() {
    return {
      dialogStates: [],
      players: [],
      playerAttributes: [],
      playerStats: [],
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
    getPositionString(positionNumber) {
      const positionMap = {
        '-1': 'NONE',
        '0': 'GK',
        '1': 'LB',
        '2': 'CB',
        '3': 'RB',
        '4': 'DM',
        '5': 'CM',
        '6': 'CAM',
        '7': 'LF',
        '8': 'ST',
        '9': 'RF'
      };

      return positionMap[positionNumber] || 'ERR';
    },
    getColorClass(value) {
      if (value >= 0 && value <= 5) return 'dark-red-text';
      else if (value > 5 && value <= 10) return 'red-text';
      else if (value > 10 && value <= 15) return 'orange-text';
      else if (value > 15 && value <= 20) return 'yellow-text';
      else if (value > 20 && value <= 25) return 'green-text';
      return '';
    },
    setMatchData(data) {
      const stats = data.data;
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
    setPlayerData(data) {
      const stats = data.data;

      this.playerStats.goals = stats[0];
      this.playerStats.assists = stats[1];
      this.playerStats.shots = stats[2];
      this.playerStats.shotsOT = stats[3];
      this.playerStats.touches = stats[4];
      this.playerStats.passesAttempt = stats[5];
      this.playerStats.passesCompleted = stats[6];
      this.playerStats.shortPasses = stats[7];
      this.playerStats.longPasses = stats[8];
      this.playerStats.throughPasses = stats[9];
      this.playerStats.dribblesAttempt = stats[10];
      this.playerStats.dribblesCompleted = stats[11];
      this.playerStats.interceptions = stats[12];
      this.playerStats.BLOCKS = stats[13];
    },
    selectOption(option) {
      this.selectedOption = option;
      this.controlMatch(option);
    },
    async openDialog(player) {
      this.playerAttributes = await window.api.getPlayer(player);
      this.playerAttributes.first_position = this.getPositionString(this.playerAttributes.first_position)
      this.playerAttributes.second_position = this.getPositionString(this.playerAttributes.second_position)
      if (this.playerAttributes.second_position === 'NONE')
        this.playerAttributes.second_position = '';
      else
        this.playerAttributes.first_position += ', '

      this.controlMatch('STATS ' + player);
    }
  },
  async mounted() {
    if (!window.api) {
      console.log('API is not available in the window object.');
      return;
    }

    try {
      this.players = await window.api.getPlayers(this.homeTeam, this.awayTeam);
    }
    catch (error) {
      console.log('Failed to fetch players:', error);
    }

    this.dialogs = Array(this.players.length).fill(false);

    window.api.onUpdateData((data) => {
      this.setMatchData(data);
    });

    window.api.onPlayerData((data) => {
      console.log(data);
      this.setPlayerData(data);
    })
  },
};
</script>

<style scoped>
.chosen {
  text-decoration: underline;
  color: blue;
  font-weight: bold;
}

.dark-red-text {
  color: #930303;
}

.red-text {
  color: #F44336;
}

.orange-text {
  color: #FF9800;
}

.yellow-text {
  color: #FFEB3B;
}

.green-text {
  color: #4CAF50;
}
</style>
