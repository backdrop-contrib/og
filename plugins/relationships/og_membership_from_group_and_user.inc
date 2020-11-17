<?php


/**
 * @file
 * Plugin to provide a relationship handler for OG membership from group and
 * user.
 */

/**
 * Return a new context based on an existing context.
 */
function og_membership_from_group_and_user_context($context, $conf) {
  list($node_context, $user_context) = $context;

  // If unset it wants a generic, unfilled context, which is just NULL.
  if (empty($node_context->data)) {
    return _og_context_create_empty('entity:og_membership', NULL);
  }

  if (empty($user_context) || empty($user_context->data)) {
    return;
  }

  $node = $node_context->data;
  $account = clone $user_context->data;

  if ($og_membership = og_get_membership('node', $node->nid, 'user', $account->uid)) {
    return _og_context_create('entity:og_membership', $og_membership);
  }
}

/**
  *
  * Helper functions below plucked from ctools.
  *
  */
  
  
/**
 * Create a new context object.
 *
 * @param string $type
 *   The type of context to create; this loads a plugin.
 * @param mixed $data
 *   The data to put into the context.
 * @param $conf
 *   A configuration structure if this context was created via UI.
 *
 * @return og_context
 *   A $context or NULL if one could not be created.
 */
function _og_context_create($type, $data = NULL, $conf = FALSE) {
  $plugin = _og_get_context($type);

  if ($function = ctools_plugin_get_function($plugin, 'context')) {
    return $function(FALSE, $data, $conf, $plugin);
  }
}

/**
 * Create an empty context object.
 *
 * Empty context objects are primarily used as placeholders in the UI where
 * the actual contents of a context object may not be known. It may have
 * additional text embedded to give the user clues as to how the context
 * is used.
 *
 * @param $type
 *   The type of context to create; this loads a plugin.
 *
 * @return og_context
 *   A $context or NULL if one could not be created.
 */
function _og_context_create_empty($type) {
  $plugin = _og_get_context($type);
  if ($function = ctools_plugin_get_function($plugin, 'context')) {
    $context = $function(TRUE, NULL, FALSE, $plugin);
    if (is_object($context)) {
      $context->empty = TRUE;
    }

    return $context;
  }
}

/**
 * Fetch metadata on a specific context plugin.
 *
 * @param string $context
 *   Name of a context.
 *
 * @return array
 *   An array with information about the requested panel context.
 */

function _og_get_context($context) {
  $plugin = ctools_get_plugins('ctools', 'contexts', $context);

  return $plugin;
}
