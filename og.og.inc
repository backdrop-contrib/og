<?php

/**
 * @file
 * Organic Groups plugins declarations.
 *
 */

/**
 * Implements hook_og_info().
 *
 */

function og_og_info() {
  $info = array(
    'relationships' => array(
      'og_membership_from_group_and_user' => array(
        'title' => t('OG membership from a group node and user'),
        'keyword' => 'og_membership',
        'description' => t('Get the OG membership context from an a node that is a group and user.'),
        'required context' => array(
          new og_context_required(t('Node'), 'node'),
          new og_context_required(t('User'), 'user'),
        ),
        'context' => 'og_membership_from_group_and_user_context',
        'file' => 'plugins/access/og_perm.inc',
      ),
    ),
  );

  if (module_exists('entityreference_prepopulate')) {
    $info['content_types'] = array(
      'node_create_links' => array(
        'title' => t('Content create links'),
        'description' => t('Crafted links to create content (nodes) for a given group.'),
        'required context' => new og_context_required(t('Node'), 'node'),
        'category' => t('Organic groups'),
        'defaults' => array(
          'types' => array(),
          'field_name' => OG_AUDIENCE_FIELD,
        ),
        'file' => 'plugins/content_types/node_create_links.inc',
      ),
    );
  }
  
  return $info;
}