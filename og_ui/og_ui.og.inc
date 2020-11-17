<?php

/**
 * @file
 * Organic Groups UI plugins declarations.
 *
 */

/**
 * Implements hook_og_info().
 *
 */

function og_ui_og_info() {
  $info = array(
    'content_types' = array(
      'membership_links' => array(
        'title' => t('Membership links'),
        'description' => t('Show membership unsubscribe and subscribe link(s) to a given group.'),
        'required context' => new og_context_required(t('OG Group'), 'entity:group'),
        'defaults' => array(
          'membership_types' => array(),
          'include_name' => FALSE,
        ),
        'category' => t('Organic groups'),
        'file' => 'plugins/content_types/membership_links/membership_links.inc',
      ),
    ),
  );
  
  return $info;
}