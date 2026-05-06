/**
 * UI Controller for DMRC Visualizer
 */
export const UI = {
    toggleLoading: (show) => {
        document.getElementById('loading-overlay').classList.toggle('hidden', !show);
    },
    updateTooltip: (station) => {
        const tooltip = document.getElementById('station-tooltip');
        if (station) {
            tooltip.classList.replace('opacity-0', 'opacity-100');
            document.getElementById('tooltip-name').textContent = station.name;
        } else {
            tooltip.classList.replace('opacity-100', 'opacity-0');
        }
    }
};
