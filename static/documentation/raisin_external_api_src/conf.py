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
html_show_sourcelink = True
html_show_sphinx = False

# Copy logo from raisin_master if available
import os
logo_path = "/home/junje/raionrobotics-webpage/static/documentation/raisin_master/v_0_2_2/_static/logo_f2-02.png"
if os.path.exists(logo_path):
    html_logo = logo_path

autosectionlabel_prefix_document = True
autosectionlabel_maxdepth = 4
