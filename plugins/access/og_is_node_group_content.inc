<?php


/**
 * @file
 * Plugin to provide access control based on existence of a specified context.
 */

/**
 * Check for access.
 */
function og_is_node_group_content_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  if (empty($context) || empty($context->data) || empty($context->data->type)) {
    return FALSE;
  }

  return og_is_group_content_type('node', $context->data->type);
}

/**
 * Provide a summary description based upon the specified context
 */
function og_is_node_group_content_summary($conf, $context) {
  return t('@identifier is an OG group content', array('@identifier' => $context->identifier));
}
