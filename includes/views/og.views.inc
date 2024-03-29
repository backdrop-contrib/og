<?php
/**
 * @file
 * Provides support for the Views module.
 */

/**
 * Implements hook_views_data_alter().
 */
function og_views_data_alter(&$data) {
  $group_content_entities = og_get_all_group_content_entity();
  $group_entity_types = og_get_all_group_entity();

  foreach (entity_get_info() as $entity_type => $info) {
    if (empty($group_content_entities[$entity_type]) && empty($group_entity_types[$entity_type])) {
      continue;
    }

    // OG membership relationship.
    $data[$info['base table']]['og_membership_rel'] = array(
      'group' => t('OG membership'),
      'title' => t('OG membership from @entity', array('@entity' => ucfirst($info['label']))),
      'help' => t('The OG membership associated with the @entity entity.', array('@entity' => ucfirst($info['label']))),
      'relationship' => array(
        // Pass the entity to the handler.
        'entity' => $entity_type,
        'handler' => 'og_handler_relationship',
        'label' => t('OG membership from @entity', array('@entity' => $entity_type)),
        'base' => 'og_membership',
        'base field' => 'etid',
        'relationship field' => $info['entity keys']['id'],
      ),
    );

    // The OG membership group.
    $data[$info['base table']]['og_membership_rel_group'] = array(
      'group' => t('OG membership'),
      'title' => t('OG membership from @entity group', array('@entity' => ucfirst($info['label']))),
      'help' => t('The OG membership associated with the @entity group', array('@entity' => ucfirst($info['label']))),
      'relationship' => array(
        'group_type' => $entity_type,
        'handler' => 'og_handler_relationship',
        'label' => t('OG membership from @entity group', array('@entity' => $entity_type)),
        'base' => 'og_membership',
        'base field' => 'gid',
        'relationship field' => $info['entity keys']['id'],
      ),
    );

    // OG membership's related entity.
    $data['og_membership']['og_membership_related_' . $entity_type] = array(
      'group' => t('OG membership'),
      'title' => t('@entity from OG membership', array('@entity' => ucfirst($info['label']))),
      'help' => t('The @entity entity that is associated with the OG membership.', array('@entity' => $info['label'])),
      'relationship' => array(
        'entity' => $entity_type,
        'handler' => 'og_handler_relationship',
        'label' => t('@entity from OG membership', array('@entity' => $entity_type)),
        'base' => $info['base table'],
        'base field' => $info['entity keys']['id'],
        'relationship field' => 'etid',
      ),
    );

    // OG membership's related group.
    $data['og_membership']['og_membership_related_' . $entity_type . '_group'] = array(
      'group' => t('OG membership'),
      'title' => t('Group @entity from OG membership', array('@entity' => ucfirst($info['label']))),
      'help' => t('The @entity group that is associated with the OG membership.', array('@entity' => $info['label'])),
      'relationship' => array(
        'group_type' => $entity_type,
        'handler' => 'og_handler_relationship',
        'label' => t('Group @entity from OG membership', array('@entity' => $entity_type)),
        'base' => $info['base table'],
        'base field' => $info['entity keys']['id'],
        'relationship field' => 'gid',
      ),
    );
  }

  $data['field_data_group_audience']['user_roles'] = array(
    'group' => t('Group'),
    'title' => t('Group user roles'),
    'help' => t('Show all the roles a user belongs to in a group.'),
    // This is a dummy field, so point it to a real field that we need - the
    // group ID
    'real field' => 'group_audience_gid',
    'field' => array(
      'handler' => 'og_handler_field_user_roles',
    ),
  );

  $data['field_data_group_audience']['og_permissions'] = array(
    'group' => t('Group'),
    'title' => t('Group permissions'),
    'help' => t('Filter by group permissions.'),
    // This is a dummy field, so point it to a real field that we need - the
    // group ID
    'real field' => 'group_audience_gid',
    'field' => array(
      'handler' => 'og_handler_field_group_permissions',
    ),
  );
}

/**
 * Implements hook_views_plugins().
 */
function og_views_plugins() {
  return array(
    'argument validator' => array(
      'og' => array(
        'title' => t('OG group'),
        'handler' => 'og_plugin_argument_validate_group',
        'path' => backdrop_get_path('module', 'og') . '/includes/views/handlers',
      ),
    ),
    'argument default' => array(
      'og_user_groups' => array(
        'title' => t('The OG groups of the logged in user'),
        'handler' => 'og_plugin_argument_default_user_groups',
      ),
    ),
  );
}
