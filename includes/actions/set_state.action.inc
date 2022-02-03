<?php

/**
 * @file
 * Action to set the state of a user in a group.
 */

/**
 * Implements hook_action_info().
 */
function og_set_state_action_info() {
  return array('og_set_state_action' => array(
      'type' => 'og_membership',
      'label' => t('Modify membership status'),
      'callback' => 'og_set_state_action_select',
      'file' => 'set_state.action.inc',
    ));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function og_set_state_action_select($og_membership, &$context) {
  $_SESSION['og_set_state_action']['state_orig'] = $og_membership->state;
  $_SESSION['og_set_state_action']['group_type'] = $og_membership->group_type;
  $_SESSION['og_set_state_action']['gid'] = $og_membership->gid;
  if ($og_membership->entity_type == 'user') {
    $_SESSION['og_set_state_action']['uids'][] = $og_membership->etid;
  }
  else {
    $_SESSION['og_set_state_action']['uids'] = array();
  }
  $context['redirect'] = 'og/set_state';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function og_set_state_action_form($og_membership, &$context) {
  // $_SESSION['og_set_state_action'] =
  $form['state'] = array(
    '#type' => 'select',
    '#title' => t('State'),
    '#description' => t('Choose the state to set for the selected users in the group.'),
    '#options' => og_group_content_states(),
    '#default_value' => OG_STATE_ACTIVE,
    '#required' => TRUE,
  );
  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function og_set_state_action_submit($form, $form_state) {
  $state = $form_state['values']['state']);
  $state_orig = $_SESSION['og_set_state_action']['state_orig'];
  $group_type = $_SESSION['og_set_state_action']['group_type'];
  $gid = $_SESSION['og_set_state_action']['gid'];
  // @todo: process multiple UIDs; see user_multiple_cancel_confirm_submit()
  //        for a model on how to do this.

  if ($state_orig == $state) {
    return;
  }

  // Don't process the group manager, if exists.
  $group = entity_load($group_type, $gid);
  if (!empty($group->uid) && $group->uid == $og_membership->etid && $og_membership->entity_type == 'user') {
    return;
  }

  $og_membership->state = $state;
  $og_membership->save();
}
