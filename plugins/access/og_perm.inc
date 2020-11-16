<?php


/**
 * @file
 * Plugin to provide access control based on user permission strings in a group.
 */

/**
 * Settings form for the 'by perm' access plugin
 */
function og_perm_ctools_access_settings($form, &$form_state, $conf) {
  $perms = array();
  // Get list of permissions
  foreach (og_get_permissions() as $perm => $value) {
    // By keeping them keyed by module we can use optgroups with the
    // 'select' type.
    $perms[$value['module']][$perm] = $value['title'];
  }

  $form['settings']['perm'] = array(
    '#type' => 'select',
    '#options' => $perms,
    '#title' => t('Group permission'),
    '#default_value' => $conf['perm'],
    '#description' => t('Only users with the selected permission flag, in the specified group, will be able to access this.'),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Check for access.
 */
function og_perm_ctools_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  list($user_context, $node_context) = $context;
  if (empty($user_context) || empty($user_context->data)) {
    return;
  }

  if (empty($node_context) || empty($node_context->data)) {
    return;
  }

  $account = clone $user_context->data;
  $node = $node_context->data;

  return og_user_access('node', $node->nid, $conf['perm'], $account);
}

/**
 * Provide a summary description based upon the checked permissions.
 */
function og_perm_ctools_access_summary($conf, $context) {
  list($user_context, $node_context) = $context;

  $permissions = og_get_permissions();
  return t('@identifier has "@perm in @group group"', array('@identifier' => $user_context->identifier, '@perm' => $permissions[$conf['perm']]['title'], '@group' => $node_context->identifier));
}

