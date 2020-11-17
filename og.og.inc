<?php

/**
 * @file
 * Organic Groups plugins declarations.
 *
 */

/**
 * Implements hook_og_info().
 *
 * The format of the plugin info:
 *
 *   $info['MACHINE_NAME'] = array(
 *   'form' => 'og_MACHINE_NAME_form',
 *   'callback' => 'og_MACHINE_NAME_callback',
 *   'name' => 'NAME',
 *   'multi' => 'loop', // loop or direct
 *   'single' => 'skip', // optional
 *   'category' => 'CATEGORY',
 *   'default description' => 'Description here', // optional
 *   'handler' => array(
 *     'file_type' => 'inc',
 *     'file_name' => 'MACHINE_NAME',
 *     'path' => 'plugins/', // usually plugins/ directory
 *     'module' => MODULE_NAME,
 *   ),
 * );
 */

function og_og_info() {
  $info = array(
    'access' = array(
      'og_is_node_group_content' => array(
        'title' => t("OG: Node is an OG group content"),
        'description' => t('Control access by whether or not a node is of type "Group content".'),
        'callback' => 'og_is_node_group_content_check',
        'summary' => 'og_is_node_group_content_summary',
        'required context' => new og_context_required(t('Node'), 'node'),
        'file' => 'plugins/access/og_is_node_group_content.inc',
      ),
      'og_is_node_group' => array(
        'title' => t("OG: Node is an OG group"),
        'description' => t('Control access by whether or not a node is of type "Group".'),
        'callback' => 'og_is_node_group_check',
        'summary' => 'og_is_node_group_summary',
        'required context' => new og_context_required(t('Node'), 'node'),
        'file' => 'plugins/access/og_is_node_group.inc',
      ),
      'og_member' => array(
        'title' => t("OG: user membership in group"),
        'description' => t('Control access by group membership.'),
        'callback' => 'og_member_access_check',
        'default' => array('state' => OG_STATE_ACTIVE),
        'settings form' => 'og_member_access_settings',
        'summary' => 'og_member_access_summary',
        'required context' => array(
          new og_context_required(t('User'), 'user'),
          new og_context_required(t('Node'), 'node'),
        ),
        'file' => 'plugins/access/og_member.inc',
      ),
      'og_perm' => array(
        'title' => t("OG: user permission in group"),
        'description' => t('Control access by group permission string.'),
        'callback' => 'og_perm_access_check',
        'default' => array('perm' => ''),
        'settings form' => 'og_perm_access_settings',
        'summary' => 'og_perm_access_summary',
        'required context' => array(
          new og_context_required(t('User'), 'user'),
          new og_context_required(t('Node'), 'node'),
        ),
        'file' => 'plugins/access/og_perm.inc',
      ),
    ),
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
    ),
  }
  
  return $info;
}