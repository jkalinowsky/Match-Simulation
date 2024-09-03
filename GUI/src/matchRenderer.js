import { createApp } from 'vue';
import Match from './components/Match.vue';
import router from "@/router";
import 'vuetify/styles'
import { createVuetify } from 'vuetify'
import * as components from 'vuetify/components'
import * as directives from 'vuetify/directives'

const vuetify = createVuetify({
    components,
    directives,
})

createApp(Match)
    .use(router)
    .use(vuetify)
    .mount('#app');