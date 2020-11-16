<?php


/**
 * @file
 * Plugin to provide access control based on existence of a specified context.
 */

/**
 * Check for access.
 */
function og_is_node_group_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  if (empty($context) || empty($context->data) || empty($context->data->nid)) {
    return FALSE;
  }

  return og_is_group('node', $context->data);
}

/**
 * Provide a summary description based upon the specified context
 */
function og_is_node_group_summary($conf, $context) {
  return t('@identifier is an OG group', array('@identifier' => $context->identifier));
}
