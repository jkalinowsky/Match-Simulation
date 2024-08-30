import { createRouter, createWebHashHistory } from 'vue-router';
import App from '@/components/App.vue';
import Match from '@/components/Match.vue';

const routes = [
    { path: '/', component: App },
    { path: '/match', component: Match },
];

const router = createRouter({
    history: createWebHashHistory(),
    routes,
});

export default router;