# Sphinx configuration for Raisin External API Reference

extensions = [
    'sphinx.ext.autosectionlabel',
]

templates_path = ['_templates']
html_theme = "sphinx_rtd_theme"

# Project information
project = 'Raisin External API Reference'
copyright = '2024, Raion Robotics.'
author = 'Raion Robotics'
version = '1.0.0'
release = '1.0.0'

# Output settings
htmlhelp_basename = 'raisin_external_api_doc'
html_show_sourcelink = False
html_show_sphinx = False

# Disable index and search
html_use_index = False
html_domain_indices = False
html_copy_source = False

# RTD theme options
html_theme_options = {
    'display_version': False,
    'prev_next_buttons_location': None,
    'collapse_navigation': False,  # Keep sidebar expanded after navigation
    'sticky_navigation': True,     # Sidebar follows scroll
    'navigation_depth': 4,         # Show all levels in sidebar
}

# Completely disable search and index pages
html_sidebars = {
    '**': ['globaltoc.html', 'localtoc.html']
}

# Copy logo from raisin_master if available
import os
logo_path = "/home/junje/raionrobotics-webpage/static/documentation/raisin_master/v_0_2_2/_static/logo_f2-02.png"
if os.path.exists(logo_path):
    html_logo = logo_path

autosectionlabel_prefix_document = True
autosectionlabel_maxdepth = 4

# Add custom CSS
html_static_path = ['_static']
html_css_files = ['custom.css']
