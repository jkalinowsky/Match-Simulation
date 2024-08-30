import { createApp } from 'vue';
import App from './components/App.vue';
import router from './router/index'

createApp(App)
    .use(router)
    .mount('#app');