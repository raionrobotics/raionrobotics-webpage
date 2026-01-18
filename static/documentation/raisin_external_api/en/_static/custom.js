// Override RTD theme's collapse behavior for single-page docs
// Keep manually expanded items open when clicking other links

(function() {
    // Wait for DOM and RTD theme to be ready
    document.addEventListener('DOMContentLoaded', function() {
        // Store which items user manually expanded
        var manuallyExpanded = new Set();
        var STORAGE_KEY = 'rtd_expanded_items';

        // Load from sessionStorage
        try {
            var stored = sessionStorage.getItem(STORAGE_KEY);
            if (stored) {
                JSON.parse(stored).forEach(function(id) {
                    manuallyExpanded.add(id);
                });
            }
        } catch(e) {}

        function saveState() {
            try {
                sessionStorage.setItem(STORAGE_KEY, JSON.stringify(Array.from(manuallyExpanded)));
            } catch(e) {}
        }

        function getItemId(li) {
            var link = li.querySelector(':scope > a');
            return link ? link.getAttribute('href') : null;
        }

        // Override RTD theme's toggleCurrent to not collapse siblings
        if (window.SphinxRtdTheme && window.SphinxRtdTheme.Navigation) {
            var originalToggleCurrent = window.SphinxRtdTheme.Navigation.toggleCurrent;

            window.SphinxRtdTheme.Navigation.toggleCurrent = function(n) {
                var li = n.closest('li')[0];
                var id = getItemId(li);

                // Just toggle this item, don't touch siblings
                var wasExpanded = li.classList.contains('current');

                if (wasExpanded) {
                    li.classList.remove('current');
                    li.setAttribute('aria-expanded', 'false');
                    if (id) manuallyExpanded.delete(id);
                } else {
                    li.classList.add('current');
                    li.setAttribute('aria-expanded', 'true');
                    if (id) manuallyExpanded.add(id);
                }

                saveState();
            };

            // Override reset to preserve manually expanded items
            var originalReset = window.SphinxRtdTheme.Navigation.reset;

            window.SphinxRtdTheme.Navigation.reset = function() {
                // Call original reset
                originalReset.call(this);

                // Restore manually expanded items
                document.querySelectorAll('.wy-menu-vertical li').forEach(function(li) {
                    var id = getItemId(li);
                    if (id && manuallyExpanded.has(id)) {
                        li.classList.add('current');
                        li.setAttribute('aria-expanded', 'true');
                    }
                });
            };
        }

        // Restore state on page load
        setTimeout(function() {
            document.querySelectorAll('.wy-menu-vertical li').forEach(function(li) {
                var id = getItemId(li);
                if (id && manuallyExpanded.has(id)) {
                    li.classList.add('current');
                    li.setAttribute('aria-expanded', 'true');
                }
            });
        }, 100);
    });
})();
