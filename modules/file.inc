<?php
// $Id$

/**
 * @file
 * Provides FileField Paths integration with the File module.
 */

/**
 * Implements hook_filefield_paths_form_alter().
 */
function file_filefield_paths_form_alter(&$form, &$ffp) {
  if (isset($form['#field']) && $form['#field']['type'] == 'file' && isset($form['instance']['settings']['file_directory'])) {
    $ffp[$form['#field']['field_name']] = array(
      'show' => TRUE,
      'type' => $form['instance']['bundle']['#value'],
      'form_path' => &$form['instance']['ffp_'. $form['#field']['field_name']],
      'file_path_default' => $form['instance']['settings']['file_directory']['#default_value']
    );

    // Create path settings fieldset
    $ffp[$form['#field']['field_name']]['form_path'] = array(
      '#type' => 'fieldset',
      '#title' => t('File Path settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 1,
    );

    $ffp[$form['#field']['field_name']]['form_path']['file_path'] = $form['instance']['settings']['file_directory'];
    $ffp[$form['#field']['field_name']]['form_path']['file_path']['#title'] = t('File path');
    $form['instance']['settings']['file_directory']['#access'] = FALSE;
  }
}

/**
 * Implements hook_filefield_paths_form_submit().
 */
function file_filefield_paths_form_submit(&$form_state, &$ffp) {
  if (isset($form_state['values']['form_id']) && $form_state['values']['form_id'] != 'node_type_form') {
    $form_state['values']['ffp_' . $form_state['values']['instance']['field_name']] = $form_state['values']['instance']['ffp_' . $form_state['values']['instance']['field_name']];
    $ffp[$form_state['values']['instance']['field_name']] = array(
      'type' => $form_state['values']['instance']['bundle'],
    );

    $form_state['values']['instance']['settings']['file_directory'] = $form_state['values']['ffp_' . $form_state['values']['instance']['field_name']]['file_path'];
  }
}

/**
 * Implements hook_filefield_paths_get_fields().
 */
function file_filefield_paths_get_fields(&$node, &$ffp) {
  if (is_object($node)) {
    // TODO: Replace with field_info_fields() when http://drupal.org/node/613754 is fixed.
    $instances = field_info_instances('node', $node->type);
    $fields = array();
    foreach ($instances as $field_name => $instance) {
      $fields[$field_name] = field_info_field($field_name);
    }

    foreach ($fields as $name => $field) {
      if ($field['type'] == 'file' && isset($node->{$field['field_name']}) && is_array($node->{$field['field_name']})) {

        foreach ($node->{$field['field_name']}['zxx'] as $file) {
          $file = file_load($file['fid']);

          $ffp['#files'][] = array(
            'field' => (array) $file,
            'module' => $field['module'],
            'name' => $field['field_name'],
            //'new' => (!empty($file['filepath']) && !empty($file['data']['process']))
            //  ? TRUE
            //  : FALSE
            'new' => !$file->status,
          );
          //unset($file['data']['process']);

          $ffp['#types'][$field['field_name']] = TRUE;
        }

      }
    }
  }
}

/**
 * Implements hook_filefield_paths_batch_update().
 */
//function file_filefield_paths_batch_update($field_name, $type_name, &$objects) {
//  if (!empty($field_name)) {
//    $field = content_fields($field_name, $type_name);
//    $db_info = content_database_info($field);
//
//    $result = db_query(
//      "SELECT c.nid FROM {%s} c LEFT JOIN {node} n ON c.nid = n.nid WHERE c.%s IS NOT NULL
//      AND n.type = '%s'", $db_info['table'], $db_info['columns']['fid']['column'], $type_name
//    );
//
//    // Build array of Node IDs.
//    while ($node = db_fetch_object($result)) {
//      $objects[] = $node->nid;
//    }
//  }
//}

/**
 * Implements hook_filefield_paths_update().
 */
//function file_filefield_paths_update($oid, $field_name) {
//  if (!empty($field_name)) {
//    $node = node_load($oid);
//    $content_type = content_types($node->type);
//
//    // Flag files for update.
//    if (isset($node->$field_name)) {
//      foreach ($node->$field_name as &$file) {
//        if (empty($file['filepath'])) {
//          continue;
//        }
//
//        $file['data']['process'] = TRUE;
//      }
//    }
//
//    // Set Form ID.
//    $node->form_id = $node->type . '_node_form';
//
//    // Process Node.
//    filefield_paths_nodeapi($node, 'update', NULL, NULL);
//  }
//}
