// MIT Licensed (see LICENSE.md).
#pragma once

#include "Repo/include/spirv-tools/instrument.hpp"
#include "Repo/include/spirv-tools/libspirv.h"
#include "Repo/include/spirv-tools/libspirv.hpp"
#include "Repo/include/spirv-tools/linker.hpp"
#include "Repo/include/spirv-tools/optimizer.hpp"
// #/source/link
// #/source/opt
#include "Repo/source/opt/aggressive_dead_code_elim_pass.h"
#include "Repo/source/opt/amd_ext_to_khr.h"
#include "Repo/source/opt/basic_block.h"
#include "Repo/source/opt/block_merge_pass.h"
#include "Repo/source/opt/block_merge_util.h"
#include "Repo/source/opt/build_module.h"
#include "Repo/source/opt/ccp_pass.h"
#include "Repo/source/opt/cfg.h"
#include "Repo/source/opt/cfg_cleanup_pass.h"
#include "Repo/source/opt/code_sink.h"
#include "Repo/source/opt/combine_access_chains.h"
#include "Repo/source/opt/compact_ids_pass.h"
#include "Repo/source/opt/composite.h"
#include "Repo/source/opt/constants.h"
#include "Repo/source/opt/const_folding_rules.h"
#include "Repo/source/opt/convert_to_half_pass.h"
#include "Repo/source/opt/copy_prop_arrays.h"
#include "Repo/source/opt/dead_branch_elim_pass.h"
#include "Repo/source/opt/dead_insert_elim_pass.h"
#include "Repo/source/opt/dead_variable_elimination.h"
#include "Repo/source/opt/debug_info_manager.h"
#include "Repo/source/opt/decoration_manager.h"
#include "Repo/source/opt/def_use_manager.h"
#include "Repo/source/opt/desc_sroa.h"
#include "Repo/source/opt/dominator_analysis.h"
#include "Repo/source/opt/dominator_tree.h"
#include "Repo/source/opt/eliminate_dead_constant_pass.h"
#include "Repo/source/opt/eliminate_dead_functions_pass.h"
#include "Repo/source/opt/eliminate_dead_functions_util.h"
#include "Repo/source/opt/eliminate_dead_members_pass.h"
#include "Repo/source/opt/empty_pass.h"
#include "Repo/source/opt/feature_manager.h"
#include "Repo/source/opt/fix_storage_class.h"
#include "Repo/source/opt/flatten_decoration_pass.h"
#include "Repo/source/opt/fold.h"
#include "Repo/source/opt/folding_rules.h"
#include "Repo/source/opt/fold_spec_constant_op_and_composite_pass.h"
#include "Repo/source/opt/freeze_spec_constant_value_pass.h"
#include "Repo/source/opt/function.h"
#include "Repo/source/opt/graphics_robust_access_pass.h"
#include "Repo/source/opt/if_conversion.h"
#include "Repo/source/opt/inline_exhaustive_pass.h"
#include "Repo/source/opt/inline_opaque_pass.h"
#include "Repo/source/opt/inline_pass.h"
#include "Repo/source/opt/instruction.h"
#include "Repo/source/opt/instruction_list.h"
#include "Repo/source/opt/instrument_pass.h"
#include "Repo/source/opt/inst_bindless_check_pass.h"
#include "Repo/source/opt/inst_buff_addr_check_pass.h"
#include "Repo/source/opt/inst_debug_printf_pass.h"
#include "Repo/source/opt/ir_builder.h"
#include "Repo/source/opt/ir_context.h"
#include "Repo/source/opt/ir_loader.h"
#include "Repo/source/opt/iterator.h"
#include "Repo/source/opt/licm_pass.h"
#include "Repo/source/opt/local_access_chain_convert_pass.h"
#include "Repo/source/opt/local_redundancy_elimination.h"
#include "Repo/source/opt/local_single_block_elim_pass.h"
#include "Repo/source/opt/local_single_store_elim_pass.h"
#include "Repo/source/opt/log.h"
#include "Repo/source/opt/loop_dependence.h"
#include "Repo/source/opt/loop_descriptor.h"
#include "Repo/source/opt/loop_fission.h"
#include "Repo/source/opt/loop_fusion.h"
#include "Repo/source/opt/loop_fusion_pass.h"
#include "Repo/source/opt/loop_peeling.h"
#include "Repo/source/opt/loop_unroller.h"
#include "Repo/source/opt/loop_unswitch_pass.h"
#include "Repo/source/opt/loop_utils.h"
#include "Repo/source/opt/mem_pass.h"
#include "Repo/source/opt/merge_return_pass.h"
#include "Repo/source/opt/module.h"
#include "Repo/source/opt/null_pass.h"
#include "Repo/source/opt/pass.h"
#include "Repo/source/opt/passes.h"
#include "Repo/source/opt/pass_manager.h"
#include "Repo/source/opt/pch_source_opt.h"
#include "Repo/source/opt/private_to_local_pass.h"
#include "Repo/source/opt/propagator.h"
#include "Repo/source/opt/reduce_load_size.h"
#include "Repo/source/opt/redundancy_elimination.h"
#include "Repo/source/opt/reflect.h"
#include "Repo/source/opt/register_pressure.h"
#include "Repo/source/opt/relax_float_ops_pass.h"
#include "Repo/source/opt/remove_duplicates_pass.h"
#include "Repo/source/opt/remove_unused_interface_variables_pass.h"
#include "Repo/source/opt/replace_invalid_opc.h"
#include "Repo/source/opt/scalar_analysis.h"
#include "Repo/source/opt/scalar_analysis_nodes.h"
#include "Repo/source/opt/scalar_replacement_pass.h"
#include "Repo/source/opt/set_spec_constant_default_value_pass.h"
#include "Repo/source/opt/simplification_pass.h"
#include "Repo/source/opt/ssa_rewrite_pass.h"
#include "Repo/source/opt/strength_reduction_pass.h"
#include "Repo/source/opt/strip_debug_info_pass.h"
//#include "Repo/source/opt/strip_reflect_info_pass.h"
#include "Repo/source/opt/strip_nonsemantic_info_pass.h"
#include "Repo/source/opt/struct_cfg_analysis.h"
#include "Repo/source/opt/tree_iterator.h"
#include "Repo/source/opt/types.h"
#include "Repo/source/opt/type_manager.h"
#include "Repo/source/opt/unify_const_pass.h"
#include "Repo/source/opt/upgrade_memory_model.h"
#include "Repo/source/opt/value_number_table.h"
#include "Repo/source/opt/vector_dce.h"
#include "Repo/source/opt/workaround1209.h"
#include "Repo/source/opt/wrap_opkill.h"	
// #/source/util
#include "Repo/source/util/bitutils.h"
#include "Repo/source/util/bit_vector.h"
#include "Repo/source/util/hex_float.h"
#include "Repo/source/util/ilist.h"
#include "Repo/source/util/ilist_node.h"
#include "Repo/source/util/make_unique.h"
#include "Repo/source/util/parse_number.h"
#include "Repo/source/util/small_vector.h"
#include "Repo/source/util/string_utils.h"
#include "Repo/source/util/timer.h"
// #/source/val
#include "Repo/source/val/basic_block.h"
#include "Repo/source/val/construct.h"
#include "Repo/source/val/decoration.h"
#include "Repo/source/val/function.h"
#include "Repo/source/val/instruction.h"
#include "Repo/source/val/validate.h"
#include "Repo/source/val/validate_memory_semantics.h"
#include "Repo/source/val/validate_scopes.h"
#include "Repo/source/val/validation_state.h"
// #/source
#include "Repo/source/assembly_grammar.h"
#include "Repo/source/binary.h"
#include "Repo/source/cfa.h"
#include "Repo/source/diagnostic.h"
#include "Repo/source/disassemble.h"
#include "Repo/source/enum_set.h"
#include "Repo/source/enum_string_mapping.h"
#include "Repo/source/extensions.h"
#include "Repo/source/ext_inst.h"
#include "Repo/source/instruction.h"
#include "Repo/source/latest_version_glsl_std_450_header.h"
#include "Repo/source/latest_version_opencl_std_header.h"
#include "Repo/source/latest_version_spirv_header.h"
#include "Repo/source/macro.h"
#include "Repo/source/name_mapper.h"
#include "Repo/source/opcode.h"
#include "Repo/source/operand.h"
#include "Repo/source/parsed_operand.h"
#include "Repo/source/print.h"
#include "Repo/source/spirv_constant.h"
#include "Repo/source/spirv_definition.h"
#include "Repo/source/spirv_endian.h"
#include "Repo/source/spirv_fuzzer_options.h"
#include "Repo/source/spirv_optimizer_options.h"
#include "Repo/source/spirv_reducer_options.h"
#include "Repo/source/spirv_target_env.h"
#include "Repo/source/spirv_validator_options.h"
#include "Repo/source/table.h"
#include "Repo/source/text.h"
#include "Repo/source/text_handler.h"
#include "Build/DebugInfo.h"
