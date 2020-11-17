<?php

/**
 * Render callback.
 */
function og_node_create_links_content_type_render($subtype, $conf, $args, $context) {
  if (empty($context->data)) {
    return FALSE;
  }

  $node = $context->data;
  $links = og_node_create_links('node', $node->nid, $conf['field_name'], NULL, !empty($conf['types']) ? $conf['types'] : NULL);
  if (!$links) {
    return FALSE;
  }

  $module = 'og';
  $block = new stdClass();
  $block->module = $module;
  $block->title = t('Content create links');

  $block->content = $links;
  return $block;
}

/**
 * Edit form.
 */
function og_node_create_links_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $options = array();
  foreach (field_info_fields() as $field_name => $field) {
    if (!og_is_group_audience_field($field_name)) {
      continue;
    }

    if ($field['settings']['target_type'] != 'node') {
      continue;
    }
    // Get the best matching field name.
    $options[$field_name] = _og_field_label($field_name) . " ($field_name)";
  }

  $form['field_name'] = array(
    '#title' => t('Field name'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $conf['field_name'],
    '#description' => t('The group audience field to prepopulate.'),
    '#required' => TRUE,
  );

  $options = array();
  foreach (node_type_get_types() as $type) {
    if (og_is_group_content_type('node', $type->type)) {
      $options[$type->type] = check_plain($type->name);
    }
  }
  $form['types'] = array(
    '#title' => t('Restrict to content types'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => $conf['types'],
    '#description' => t('If left empty, all possible content types are shown.'),
  );
  return $form;
}

/**
 * Edit form submit callback.
 */
function og_node_create_links_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['field_name'] = $form_state['values']['field_name'];
  $form_state['conf']['types'] = array_filter($form_state['values']['types']);
}

/**
 * Returns the label of a certain field.
 *
 * Cribbed from Views.
 */
function _og_field_label($field_name) {
  $label_counter = array();
  // Count the amount of instances per label per field.
  $instances = field_info_instances();
  foreach ($instances as $entity_type) {
    foreach ($entity_type as $bundle) {
      if (isset($bundle[$field_name])) {
        $label_counter[$bundle[$field_name]['label']] = isset($label_counter[$bundle[$field_name]['label']]) ? ++$label_counter[$bundle[$field_name]['label']] : 1;
      }
    }
  }
  if (empty($label_counter)) {
    return $field_name;
  }
  // Sort the field lables by it most used label and return the most used one.
  arsort($label_counter);
  $label_counter = array_keys($label_counter);
  return $label_counter[0];
}
