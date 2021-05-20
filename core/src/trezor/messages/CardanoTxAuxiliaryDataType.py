# Automatically generated by pb2py
# fmt: off
# isort:skip_file
import protobuf as p

from .CardanoCatalystRegistrationParametersType import CardanoCatalystRegistrationParametersType

if __debug__:
    try:
        from typing import Dict, List, Optional  # noqa: F401
        from typing_extensions import Literal  # noqa: F401
    except ImportError:
        pass


class CardanoTxAuxiliaryDataType(p.MessageType):

    def __init__(
        self,
        *,
        blob: Optional[bytes] = None,
        catalyst_registration_parameters: Optional[CardanoCatalystRegistrationParametersType] = None,
    ) -> None:
        self.blob = blob
        self.catalyst_registration_parameters = catalyst_registration_parameters

    @classmethod
    def get_fields(cls) -> Dict:
        return {
            1: ('blob', p.BytesType, None),
            2: ('catalyst_registration_parameters', CardanoCatalystRegistrationParametersType, None),
        }